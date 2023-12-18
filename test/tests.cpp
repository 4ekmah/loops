/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "tests.hpp"
#include "src/common.hpp"
#include "src/reg_allocator.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <locale>
#include <sys/stat.h> //TODO(ch): *nix-only. Use winbase.h for Windows 

using namespace loops;

static inline std::string OSname()
{
    #if __LOOPS_OS == __LOOPS_LINUX
        return "Linux";
    #elif __LOOPS_OS == __LOOPS_WINDOWS
        return "Windows";
    #elif __LOOPS_OS == __LOOPS_MAC
        return "Macos";
    #else
        #error Unknown OS
    #endif
}

inline std::string toLower(const std::string& tL)
{
    std::string res = tL;
    std::transform(res.begin(), res.end(), res.begin(), [](char t) {return (char)::tolower(t); });
    return res;
}

void switch_spill_stress_test_mode_on(loops::Func& func)
{
    ContextImpl* CTX = ((FuncImpl*)getImpl(&func))->getContext();
    std::vector<int> sparam = CTX->getBackend()->parameterRegisters(RB_INT);
    std::vector<int> sreturn = CTX->getBackend()->returnRegisters(RB_INT);
    std::vector<int> scallers = CTX->getBackend()->callerSavedRegisters(RB_INT);
    std::vector<int> scallees = CTX->getBackend()->calleeSavedRegisters(RB_INT);

    std::vector<int> vparam = CTX->getBackend()->parameterRegisters(RB_VEC);
    std::vector<int> vreturn = CTX->getBackend()->returnRegisters(RB_VEC);
    std::vector<int> vcallers = CTX->getBackend()->callerSavedRegisters(RB_VEC);
    std::vector<int> vcallees = CTX->getBackend()->calleeSavedRegisters(RB_VEC);

#if __LOOPS_ARCH == __LOOPS_AARCH64
enum Aarch64Reg //DUBUG: we don't need to know here certain register distribution, there have to work exactly algorithm
{
    R0   =  0,
    R1   =  1,
    R2   =  2,
    R3   =  3,
    R4   =  4,
    R5   =  5,
    R6   =  6,
    R7   =  7,
    XR   =  8,
    R9   =  9,
    R10  = 10,
    R11  = 11,
    R12  = 12,
    R13  = 13,
    R14  = 14,
    R15  = 15,
    IP0  = 16,
    IP1  = 17,
    PR   = 18,
    R19  = 19,
    R20  = 20,
    R21  = 21,
    R22  = 22,
    R23  = 23,
    R24  = 24,
    R25  = 25,
    R26  = 26,
    R27  = 27,
    R28  = 28,
    FP   = 29,
    LR   = 30,
    SP   = 31
};

enum Aarch64VReg
{
    Q0   =  0,
    Q1   =  1,
    Q2   =  2,
    Q3   =  3,
    Q4   =  4,
    Q5   =  5,
    Q6   =  6,
    Q7   =  7,
    Q8   =  8,
    Q9   =  9,
    Q10   =  10,
    Q11   =  11,
    Q12   =  12,
    Q13   =  13,
    Q14   =  14,
    Q15   =  15,
    Q16   =  16,
    Q17   =  17,
    Q18   =  18,
    Q19   =  19,
    Q20   =  20,
    Q21   =  21,
    Q22   =  22,
    Q23   =  23,
    Q24   =  24,
    Q25   =  25,
    Q26   =  26,
    Q27   =  27,
    Q28   =  28,
    Q29   =  29,
    Q30   =  30,
    Q31   =  31,
};

    sparam = { R0, R1, R2, R3 };
    sreturn = { R0, R1, R2, R3 };
    scallees = {};
    scallers = { PR, R19, R20, R21, R22 };

    vparam = { Q0, Q1, Q2, Q3 };
    vreturn = { Q0, Q1, Q2, Q3 };
    vcallees = { };
    vcallers = { Q29, Q30, Q31 };

        //if (CTX.getPlatformName() == "AArch64")
        //     getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6, 7 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, {}, { 18, 19, 20, 21, 22 });
        //     getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6 }   , { 0, 1, 2, 3, 4, 5, 6 }   , {}, { 18, 19, 20, 21, 22 });
        //     getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4 }         , { 0, 1, 2, 3, 4 }         , {}, { 18, 19, 20, 21, 22 });
        //m_calleeSavedRegisters[RB_INT] = { |||PR, R19, R20, R21, R22|||, R23, R24, R25, R26, R27, R28 };

#elif __LOOPS_ARCH == __LOOPS_INTEL64
    //DUBUG read comments lower and clear after: 
//1.) We are taking first "default_params_amount" registers from params basket.
//2.) Take intersect with return basket. If it is empty, take first register.
//3.) Take 0 of callee_saved registers
//4.) Take common_amount - sizeof(union of p.1 and p.2) last registers from caller_saved.
//5.) Now take "params_amount" from params basket and correct accordingly to p.2 return registers.
    const size_t common_amount = 9; //Well, tell me why?
    const size_t default_params_amount = 4;
    const size_t params_amount = std::min((size_t)std::max(func.signature(), 4), sparam.size()); //Accordingly to function signature. Can be more than default_params_amount, but not vice versa.
    {
        size_t callees_amount = common_amount - (default_params_amount + (std::count(sparam.begin(), sparam.begin() + default_params_amount, sreturn[0]) == 0 ? 1 : 0));
        scallers.clear();
        if(callees_amount < scallees.size())
            scallees.erase(scallees.begin(), scallees.begin() + scallees.size() - callees_amount);
        if(params_amount < sparam.size())
            sparam.erase(sparam.begin() + params_amount, sparam.end());
        std::set<int> ireturn({sreturn[0]});
        for(int retreg : sreturn)
            if(std::count(sparam.begin(), sparam.end(), retreg) > 0)
                ireturn.insert(retreg);
        for(int regnum = (int)sreturn.size() - 1; regnum >= 0; regnum--)
            if(ireturn.count(sreturn[regnum]) == 0)
                sreturn.erase(sreturn.begin()+regnum);
    // #if __LOOPS_OS == __LOOPS_WINDOWS
    //     sparam = { RCX, RDX, R8, R9 };
    //     sreturn = { RAX };
    //     scallees = {};
    //     scallers = { R12, R13, R14, R15 };
    }
#else
#error Unknown CPU
#endif
    getImpl(&func)->overrideRegisterSet(RB_INT, sparam, sreturn, scallers, scallees);
    getImpl(&func)->overrideRegisterSet(RB_VEC, vparam, vreturn, vcallers, vcallees);
}

void direct_translation_on(loops::Func& func)
{
    getImpl(&func)->directTranslationOn();
}

bool checkListingEquality(const std::string& curL, const std::string& refL, const std::string& errMes, std::ostream& out_stream)
{
    size_t rowNum = 1;
    size_t colNum = 1;
    size_t curPos = 0;
    size_t refPos = 0;
    const size_t curSiz = curL.size();
    const size_t refSiz = refL.size();
    bool result = true;
    std::string mainErrorMes = "    " + errMes +": ";
    while(true)   //TODO(ch): Line vs line comparing and avoid spaces duplicates.
    {
        if(curPos == curSiz && refSiz!=curSiz)
        {
            mainErrorMes += "Unexpected end of listing.";
            result = false;
            break;
        }
        if(refPos == refSiz && refSiz!=curSiz)
        {
            mainErrorMes += "Extra data in listing.";
            result = false;
            break;
        }
        if(curPos == curSiz)
            break;
        if(curL[curPos] != refL[refPos])
        {
            mainErrorMes += "Listing doesn't equal to reference.";
            result = false;
            break;
        }
        if(curL[curPos] == '\n') //TODO(ch): What about windows style endlines?
        {
            colNum = 1;
            ++rowNum;
        }
        else
            ++colNum;
        ++curPos;
        ++refPos;
    }
    if(!result)
    {
        out_stream<<mainErrorMes.c_str()<<std::endl;
        out_stream<<"        Row: " << rowNum<< " | Col: "<<colNum<<std::endl;
        size_t curErrEndPos = curL.find_first_of('\n', curPos); //TODO(ch): What about windows style endlines?
        size_t refErrEndPos = refL.find_first_of('\n', refPos);
        colNum-=1;
        curPos -= colNum;
        refPos -= colNum;
        std::string curProblemLine = curL.substr(curPos, curErrEndPos - curPos);
        std::string refProblemLine = refL.substr(refPos, refErrEndPos - refPos);
        out_stream<<"        Current listing line: \""<<curProblemLine<<"\""<<std::endl;
        out_stream<<"        Reference line      : \""<<refProblemLine<<"\""<<std::endl;
    }
    return result;
}

inline bool fileexists(const std::string& name) //TODO(ch): use GetFileAttributes for windows
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

bool intermediate_representation_is_stable(loops::Func func, std::string& errmessage)
{
    std::stringstream errstream;
    const std::string func_name = func.name();
    const std::string tarcname = ((FuncImpl*)getImpl(&func))->getContext()->getPlatformName();
    const std::string arcOSsuffix = toLower(tarcname) + "/" + toLower(OSname());
    const std::string irfilename = std::string(LOOPS_TEST_DIR"/refasm/") + arcOSsuffix + "/bytecode/" + func_name + ".tst"; //DUBUG: change dir: bytecode -> IR
    bool result = true;
    bool rewrite = false;
    std::string irtext;
    ::std::ostringstream irstream(irtext, ::std::ios::out);
    func.printBytecode(irstream);
    irtext = irstream.str();
    if(fileexists(irfilename))
    {
        std::ifstream refstream(irfilename.c_str(), ::std::ios::in);
        if(!refstream.good())
        {
            errstream << "    Intermediate representation check failed: cannot open file: \""<<irfilename<<"\"."<<std::endl;
            rewrite = RECREATE_REFERENCE_TEXTS;
        }
        else
        {
            std::string irref((std::istreambuf_iterator<char>(refstream)), std::istreambuf_iterator<char>());
            if(!checkListingEquality(irtext, irref, "Intermediate representation check failed", errstream))
            {
                rewrite = RECREATE_REFERENCE_TEXTS;
                result = false;
            }
        }
    }
    else
    {
        errstream<<"    Intermediate representation check failed: file doesn't exist: \""<<irfilename<<"\"."<<std::endl;
        rewrite = RECREATE_REFERENCE_TEXTS;
        result = false;
    }
    if(rewrite)
    {
        std::ofstream rwrtstream(irfilename.c_str(), ::std::ios::out);
        if(!rwrtstream.good())
            errstream<<"    Intermediate representation check failed: cannot open file for rewrite: \""<<irfilename<<"\"."<<std::endl;
        rwrtstream<<irtext;
        rwrtstream.close();
    }
    errmessage = errstream.str();
    return result;
}

bool assembly_is_stable(loops::Func func, std::string& errmessage)
{
    std::stringstream errstream;
    const std::string func_name = func.name();
    const std::string tarcname = ((FuncImpl*)getImpl(&func))->getContext()->getPlatformName();
    const std::string arcOSsuffix = toLower(tarcname) + "/" + toLower(OSname());
    const std::string afilename = std::string(LOOPS_TEST_DIR"/refasm/") + arcOSsuffix + "/" + func_name + ".tst";
    bool result = true;
    bool rewrite = false;
    std::string atext;
    ::std::ostringstream astream(atext, ::std::ios::out);
    func.printAssembly(astream);
    atext = astream.str();
    if(fileexists(afilename))
    {
        std::ifstream refstream(afilename.c_str(), ::std::ios::in);
        if(!refstream.good())
        {
            errstream<<"    Check for "<<tarcname<<" failed: cannot open file: \""<<afilename<<"\"."<<std::endl;
            rewrite = RECREATE_REFERENCE_TEXTS;
        }
        else
        {
            std::string bcref((std::istreambuf_iterator<char>(refstream)), std::istreambuf_iterator<char>());
            if(!checkListingEquality(atext, bcref, "Check for "+tarcname+" failed", errstream))
            {
                rewrite = RECREATE_REFERENCE_TEXTS;
                result = false;
            }
        }
    }
    else
    {
        errstream<<"    Check for "<<tarcname<<" failed: file doesn't exist: \""<<afilename<<"\"."<<std::endl;
        rewrite = RECREATE_REFERENCE_TEXTS;
        result = false;
    }
    if(rewrite)
    {
        std::ofstream rwrtstream(afilename.c_str(), ::std::ios::out);
        if(!rwrtstream.good())
            errstream<<"    Check for "<<tarcname<<" failed: cannot open file for rewrite: \""<<afilename<<"\"."<<std::endl;
        rwrtstream<<atext;
        rwrtstream.close();
    }
    errmessage = errstream.str();
    return result;
}

loops::Func* get_assembly_reg_param(loops::Func& func)
{
    return getImpl(&func);
}

bool memok(uint8_t* canvas, int64_t w, int64_t h)
{
    for(int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            if (canvas[i * w + j] != 0)
            {
                std::cout << "    Memory writing violation at output [" << -1 << ", " << i << ", " << j << "]" << std::endl;
                return false;
            }
            if(canvas[2*h*w + i*w + j] != 0)
            {
                std::cout << "    Memory writing violation at output ["<< 1 <<", "<< i <<", "<< j<<"]"<<std::endl;
                return false;
            }
        }
    return true;
}

::std::ostringstream test_stream;

std::ostream& get_test_ostream()
{
    return test_stream;
}

void reset_test_ostream()
{
    test_stream = ::std::ostringstream();
}

std::string get_test_ostream_result()
{
    return test_stream.str();
}
