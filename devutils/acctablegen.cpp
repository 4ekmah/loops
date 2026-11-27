/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "src/backend.hpp"
#include "src/backend_aarch64.hpp"
#include "src/backend_intel64.hpp"
#include "src/backend_riscV.hpp"
#include "src/acceleration.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace loops
{
class AccelerationTableGenerator
{
public:
    AccelerationTableGenerator(Backend& backend, const std::string& directory)
        : m_backend(backend), m_directory(directory), m_supported(0), m_conflicts(0)
    {}

    void generate();
    void write();

private:
    struct RegisterSets
    {
        RegisterSets() : input(0), output(0), seen(false), conflict(false) {}
        uint64_t input;
        uint64_t output;
        bool seen;
        bool conflict;
    };

    void enumerateShapes(int opcode, int args_size);
    void process(const Syntop& op, bool allow_missing_translation = false);
    void processTypePairs(const Syntop& op);
    bool needsTypePairs(int opcode) const;
    Syntop makeOperation(int opcode, const std::array<int, Syntop::SYNTOP_ARGS_MAX>& tags,
                         int args_size, int elemtype, int value_variant) const;
    int64_t immediateValue(int opcode, int argnum, int value_variant) const;

    Backend& m_backend;
    std::string m_directory;
    std::unordered_map<uint64_t, RegisterSets> m_results;
    size_t m_supported;
    size_t m_conflicts;
    fasthash m_input;
    fasthash m_output;
};

Syntop AccelerationTableGenerator::makeOperation(int opcode,
    const std::array<int, Syntop::SYNTOP_ARGS_MAX>& tags, int args_size,
    int elemtype, int value_variant) const
{
    std::vector<Arg> args;
    args.reserve(args_size);
    for (int argnum = 0; argnum < args_size; ++argnum)
    {
        Arg arg;
        switch (tags[argnum])
        {
        case Arg::EMPTY:
            break;
        case Arg::IREG:
        case Arg::VREG:
        {
            int regidx = 0;
            if (opcode == OP_ARM_LDP || opcode == OP_ARM_STP || opcode == VOP_LD2)
                regidx = argnum;
            arg = argReg(tags[argnum] == Arg::IREG ? RB_INT : RB_VEC, regidx);
            break;
        }
        case Arg::IIMMEDIATE:
            arg = argIImm(immediateValue(opcode, argnum, value_variant));
            break;
        case Arg::ISPILLED:
        case Arg::VSPILLED:
            arg = argSpilled(tags[argnum] == Arg::ISPILLED ? RB_INT : RB_VEC, value_variant);
            break;
        default:
            throw std::logic_error("Unknown argument tag in acceleration table generator.");
        }
        arg.elemtype = elemtype;
        args.push_back(arg);
    }
    return Syntop(opcode, args);
}

int64_t AccelerationTableGenerator::immediateValue(int opcode, int argnum, int value_variant) const
{
    const bool condition = ((opcode == OP_SELECT || opcode == OP_IVERSON) && argnum == 1) ||
                           (opcode == OP_JCC && argnum == 0) ||
                           ((opcode == OP_ARM_CINC || opcode == OP_ARM_CNEG) && argnum == 2);
    return condition ? (value_variant ? OP_NE : OP_EQ) : value_variant;
}

bool AccelerationTableGenerator::needsTypePairs(int opcode) const
{
    switch (opcode)
    {
    case VOP_TRUNC:
    case VOP_FLOOR:
    case VOP_CAST:
    case VOP_REINTERPRET:
    case VOP_CAST_LOW:
    case VOP_CAST_HIGH:
    case VOP_SHRINK:
    case VOP_ARM_SHRINK_LOW:
    case VOP_ARM_SHRINK_HIGH:
    case VOP_REDUCE_WSUM:
    case VOP_SELECT:
    case VOP_GT:
    case VOP_GE:
    case VOP_LT:
    case VOP_LE:
    case VOP_NE:
    case VOP_EQ:
        return true;
    default:
        return false;
    }
}

void AccelerationTableGenerator::processTypePairs(const Syntop& op)
{
    for (int first = TYPE_U8; first <= TYPE_BOOLEAN; ++first)
        for (int second = TYPE_U8; second <= TYPE_BOOLEAN; ++second)
        {
            Syntop variant = op;
            switch (op.opcode)
            {
            case VOP_SELECT:
                if (op.args_size != 4)
                    continue;
                for (int argnum = 0; argnum < op.args_size; ++argnum)
                    variant.args[argnum].elemtype = argnum == 1 ? second : first;
                break;
            case VOP_GT:
            case VOP_GE:
            case VOP_LT:
            case VOP_LE:
            case VOP_NE:
            case VOP_EQ:
                if (op.args_size != 3)
                    continue;
                variant.args[0].elemtype = first;
                variant.args[1].elemtype = second;
                variant.args[2].elemtype = second;
                break;
            case VOP_SHRINK:
                if (op.args_size != 3)
                    continue;
                variant.args[0].elemtype = first;
                variant.args[1].elemtype = second;
                variant.args[2].elemtype = second;
                break;
            case VOP_REDUCE_WSUM:
                if (op.args_size != 2 && op.args_size != 3)
                    continue;
                variant.args[0].elemtype = first;
                variant.args[1].elemtype = second;
                if (op.args_size == 3)
                    variant.args[2].elemtype = first;
                break;
            default:
                if (op.args_size != 2)
                    continue;
                variant.args[0].elemtype = first;
                variant.args[1].elemtype = second;
                break;
            }
            process(variant);
        }
}

void AccelerationTableGenerator::process(const Syntop& op, bool allow_missing_translation)
{
    try
    {
        bool found = false;
        m_backend.m_s2slookup(&m_backend, op, found);
        if (!found && !allow_missing_translation)
            return;
        const uint64_t input = packFastsets(
            m_backend.getUsedRegistersIdxsUncached(op, RB_INT, AF_INPUT),
            m_backend.getUsedRegistersIdxsUncached(op, RB_VEC, AF_INPUT));
        const uint64_t output = packFastsets(
            m_backend.getUsedRegistersIdxsUncached(op, RB_INT, AF_OUTPUT),
            m_backend.getUsedRegistersIdxsUncached(op, RB_VEC, AF_OUTPUT));
        const uint64_t io = packFastsets(
            m_backend.getUsedRegistersIdxsUncached(op, RB_INT, AF_INPUT | AF_OUTPUT),
            m_backend.getUsedRegistersIdxsUncached(op, RB_VEC, AF_INPUT | AF_OUTPUT));
        ++m_supported;
        RegisterSets& sets = m_results[syntopKey(op)];
        if (!sets.seen)
        {
            sets.input = input;
            sets.output = output;
            sets.seen = true;
        }
        if (io != (input & output) || sets.input != input || sets.output != output)
        {
            if (!sets.conflict)
                ++m_conflicts;
            sets.conflict = true;
        }
    }
    catch (const std::exception&)
    {}
}

void AccelerationTableGenerator::enumerateShapes(int opcode, int args_size)
{
    int shapes = 1;
    for (int argnum = 0; argnum < args_size; ++argnum)
        shapes *= 6;
    for (int shape = 0; shape < shapes; ++shape)
    {
        std::array<int, Syntop::SYNTOP_ARGS_MAX> tags = {};
        int encoded = shape;
        for (int argnum = 0; argnum < args_size; ++argnum)
        {
            tags[argnum] = encoded % 6;
            encoded /= 6;
        }
        for (int value_variant = 0; value_variant < 2; ++value_variant)
        {
            for (int elemtype = TYPE_U8; elemtype <= TYPE_BOOLEAN; ++elemtype)
                process(makeOperation(opcode, tags, args_size, elemtype, value_variant));
            if (needsTypePairs(opcode))
                processTypePairs(makeOperation(opcode, tags, args_size, TYPE_U8, value_variant));
        }
    }
}

void AccelerationTableGenerator::generate()
{
    for (int opcode = 0; opcode <= OP_NOINIT; ++opcode)
    {
        if (opcode == OP_DEF || opcode == VOP_DEF ||
            (opcode >= OP_IF_CSTART && opcode <= OP_CONTINUE))
            continue;
        for (int args_size = 0; args_size <= 4; ++args_size)
            enumerateShapes(opcode, args_size);
    }
    process(Syntop(OP_DEF, {argReg(RB_INT, 0)}), true);
    process(Syntop(VOP_DEF, {argReg(RB_VEC, 0)}), true);
    process(Syntop(OP_IF_CSTART, {}), true);
    process(Syntop(OP_ELIF_CSTART, {argIImm(0), argIImm(0)}), true);
    process(Syntop(OP_IF_CEND, {}), true);
    process(Syntop(OP_ELSE, {argIImm(0), argIImm(0)}), true);
    process(Syntop(OP_ENDIF, {argIImm(0)}), true);
    process(Syntop(OP_WHILE_CSTART, {argIImm(0)}), true);
    process(Syntop(OP_WHILE_CEND, {}), true);
    process(Syntop(OP_ENDWHILE, {argIImm(0), argIImm(0)}), true);
    process(Syntop(OP_BREAK, {argIImm(0)}), true);
    process(Syntop(OP_CONTINUE, {argIImm(0)}), true);
    for (int opcode = OP_CALL; opcode <= OP_CALL_NORET; ++opcode)
        for (int args_size = 5; args_size <= Syntop::SYNTOP_ARGS_MAX; ++args_size)
        {
            std::array<int, Syntop::SYNTOP_ARGS_MAX> tags = {};
            std::fill(tags.begin(), tags.end(), Arg::IREG);
            process(makeOperation(opcode, tags, args_size, TYPE_I64, 0));
        }
}

void AccelerationTableGenerator::write()
{
    std::vector<uint64_t> keys;
    keys.reserve(m_results.size());
    for (const auto& result : m_results)
        if (!result.second.conflict)
            keys.push_back(result.first);
    std::sort(keys.begin(), keys.end());
    for (uint64_t key : keys)
    {
        const RegisterSets& sets = m_results.find(key)->second;
        m_input.insert(key, sets.input);
        m_output.insert(key, sets.output);
    }
    m_input.compact();
    m_output.compact();
    std::filesystem::create_directories(m_directory);
    const std::string input_path = accelerationTableFile(m_directory, true);
    const std::string output_path = accelerationTableFile(m_directory, false);
    m_input.write(input_path);
    m_output.write(output_path);

    fasthash input_check;
    fasthash output_check;
    input_check.read(input_path);
    output_check.read(output_path);
    for (uint64_t key : keys)
        if (input_check.at(key) != m_results.find(key)->second.input ||
            output_check.at(key) != m_results.find(key)->second.output)
            throw std::runtime_error("Acceleration table readback verification failed.");

    std::cout << "Supported variants: " << m_supported << '\n'
              << "Conflicting keys: " << m_conflicts << '\n'
              << "Written pairs: " << keys.size() << '\n'
              << input_path << " (" << input_check.capacity() << " slots)\n"
              << output_path << " (" << output_check.capacity() << " slots)\n";
}
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: acctablegen <output-directory>\n";
        return 2;
    }
    try
    {
#if __LOOPS_ARCH == __LOOPS_INTEL64
        loops::Intel64Backend backend;
#elif __LOOPS_ARCH == __LOOPS_AARCH64
        loops::Aarch64Backend backend;
#else
        loops::RiscVBackend backend;
#endif
        loops::AccelerationTableGenerator generator(backend, argv[1]);
        generator.generate();
        generator.write();
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
    return 0;
}
