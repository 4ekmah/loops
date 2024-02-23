/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include <zip_file.hpp>
#include "tests.hpp"
#include "src/common.hpp"
#include "src/reg_allocator.hpp"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <set>
#include <locale>
#include <sys/stat.h> //TODO(ch): *nix-only. Use winbase.h for Windows 

using namespace loops;

#define LISTINGS_ROOT LOOPS_TEST_DIR"/refasm/listings/"

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

static inline std::string ARCHname()
{
#if __LOOPS_ARCH == __LOOPS_INTEL64
    return "Intel64";
#elif __LOOPS_ARCH == __LOOPS_AARCH64
    return "Aarch64";
#else
#error Unknown CPU
#endif
}

inline std::string toLower(const std::string& tL)
{
    std::string res = tL;
    std::transform(res.begin(), res.end(), res.begin(), [](char t) {return (char)::tolower(t); });
    return res;
}
#undef min
#undef max
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

    const size_t common_amount = 8; //Well, tell me why?
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
    }
    getImpl(&func)->overrideRegisterSet(RB_INT, sparam, sreturn, scallers, scallees);
    getImpl(&func)->overrideRegisterSet(RB_VEC, vparam, vreturn, vcallers, vcallees);
}

void direct_translation_on(loops::Func& func)
{
    getImpl(&func)->directTranslationOn();
}

struct listing_token
{
    std::string token;
    size_t line;
    size_t col;
    listing_token(const std::string& a_token, size_t a_line, size_t a_col): token(a_token), line(a_line), col(a_col){}
};

struct tokenized_line
{
    std::string wholeline;
    std::vector<listing_token> words;
    size_t line;
    tokenized_line(const std::string a_wholeline, size_t a_line): wholeline(a_wholeline), line(a_line)
    {
        size_t pos = 0;
        const size_t sz = wholeline.size();
        while(pos < sz && wholeline[pos] == ' ') pos++;
        while(pos < sz)
        {
            size_t tokenend = pos;
            while(tokenend < sz && wholeline[tokenend] != ' ') tokenend++;
            words.push_back(listing_token(std::string(wholeline.begin()+pos, wholeline.begin()+tokenend), line, pos));
            pos = tokenend;
            while(pos < sz && wholeline[pos] == ' ') pos++;
        }
    }
};

static std::vector<tokenized_line> tokenizer(const std::string& text)
{
    std::vector<tokenized_line> res;
    const size_t sz = text.size();
    size_t pos = 0;
    size_t line = 0;
    while (pos < sz)
    {
        size_t newpos = text.find('\n',pos);
        newpos = (newpos == std::string::npos ? sz : newpos);
        size_t justafter = newpos;
        res.push_back(tokenized_line(std::string(text.begin()+pos, text.begin()+justafter), line));
        pos = newpos + 1;
        line++;
    }
    while(res.size() && res.back().words.size() == 0)
        res.pop_back();
    return res;
}

std::string create_pointing_lines(size_t ptr, size_t underlying_start, size_t underlying_end)
{
    std::stringstream resstream;
    resstream << std::string(8 + underlying_start,' ') + std::string(underlying_end - underlying_start, '~') << std::endl;
    resstream << std::string(8 + ptr,' ') + "^" << std::endl;
    return resstream.str();
}

struct listing_defect
{
    std::string func_name;
    std::string pass_name;
    size_t line;
    size_t word;
    listing_defect() {}
    listing_defect(const std::string& a_func_name, const std::string& a_pass_name, size_t a_line, size_t a_word) : func_name(a_func_name)
        , pass_name(a_pass_name)
        , line(a_line)
        , word(a_word){}
    void write(std::ostream& out) const
    {
        size_t sz = func_name.size();
        out.write((char*)&sz, sizeof(sz));
        out.write(func_name.data(), sz);
        sz = pass_name.size();
        out.write((char*)&sz, sizeof(sz));
        out.write(pass_name.data(), sz);
        out.write((char*)&line, sizeof(line));
        out.write((char*)&word, sizeof(word));
    }

    static listing_defect read(std::istream& in)
    {
        listing_defect res;
        size_t sz;
        in.read((char*)&sz, sizeof(sz));
        res.func_name.resize(sz, ' ');
        in.read(res.func_name.data(), sz);
        in.read((char*)&sz, sizeof(sz));
        res.pass_name.resize(sz, ' ');
        in.read(res.pass_name.data(), sz);
        in.read((char*)&res.line, sizeof(res.line));
        in.read((char*)&res.word, sizeof(res.word));
        return res;
    }
};

bool operator<(const listing_defect& a, const listing_defect& b)
{
    if (a.func_name < b.func_name)
        return true;
    else if (a.func_name > b.func_name)
        return false;
    else if (a.pass_name < b.pass_name)
        return true;
    else if (a.pass_name > b.pass_name)
        return false;
    else if (a.line < b.line)
        return true;
    else if (a.line > b.line)
        return false;
    else if (a.word < b.word)
        return true;
    return false;
}
std::set<listing_defect> defects;

bool check_listing_equality(const std::string& curL, const std::string& refL, const std::string& errPrefix, std::ostream& out_stream, bool tolerable_defect, const std::string& func_name, const std::string& pass_name)
{
    std::string endline; {std::stringstream tmpstream; tmpstream << std::endl; endline = tmpstream.str();}
    std::vector<tokenized_line> ref_tokenized = tokenizer(refL); 
    std::vector<tokenized_line> cur_tokenized = tokenizer(curL);
    bool result = true;
    std::string main_error_line;
    std::string refline;
    std::string ref_underline;
    std::string curline;
    std::string cur_underline;

    size_t lines_amount = std::max(ref_tokenized.size(), cur_tokenized.size()); 
    for(size_t line = 0; line < lines_amount; line++)
    {
        if(line >= ref_tokenized.size())
        {
            main_error_line = errPrefix + "Listing have extra line number " + std::to_string(line + 1) + endline;
            curline = std::string("    Current line: ") + endline + "        "  + cur_tokenized[line].wholeline + endline;
            result = false;
            break;
        }

        if(line >= cur_tokenized.size())
        {
            main_error_line = errPrefix + "Listing doesn't have line number " + std::to_string(line + 1) + endline;
            curline = std::string("    Reference line: ") + endline + "        "  + ref_tokenized[line].wholeline + endline;
            result = false;
            break;
        }

        size_t words_amount = std::max(ref_tokenized[line].words.size(), cur_tokenized[line].words.size()); 
        for(size_t word = 0; word < words_amount; word++)
        {
            if(word >= ref_tokenized[line].words.size())
            {
                std::string& curword = cur_tokenized[line].words[word].token;
                main_error_line = errPrefix + "Extra token in listing on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].wholeline.size()) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].words[word].col + 1) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                cur_underline = create_pointing_lines(cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col + curword.size());
                result = false;
                break;
            }

            if(word >= cur_tokenized[line].words.size())
            {
                std::string& refword = ref_tokenized[line].words[word].token;
                main_error_line = errPrefix + "Missing token in listing on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].words[word].col + 1) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                ref_underline = create_pointing_lines(ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col + refword.size());
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].wholeline.size()) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                result = false;
                break;
            }

            std::string& refword = ref_tokenized[line].words[word].token;
            std::string& curword = cur_tokenized[line].words[word].token;
            if(refword != curword)
            {
                if (tolerable_defect)
                {
                    listing_defect defect(func_name, pass_name, line, word);
#if (RECREATE_TOLERABLE_DEFECT == true)
                    defects.insert(defect);
                    continue;
#else
                    if (defects.find(defect) != defects.end())
                        continue;
#endif
                }
                size_t inequal_pos = 0; for(;inequal_pos < refword.size() && inequal_pos < curword.size() && refword[inequal_pos] == curword[inequal_pos]; inequal_pos++);
                assert(curword.size() > 0 && refword.size() > 0);
                size_t ref_inpos = std::min(inequal_pos, refword.size() - 1);
                size_t cur_inpos = std::min(inequal_pos, curword.size() - 1);
                main_error_line = errPrefix + "Listing doesn't equal to reference on line " + std::to_string(line + 1) + endline;
                refline = std::string("    Reference line, column ") + std::to_string(ref_tokenized[line].words[word].col + 1) + ":" + endline + "        " + ref_tokenized[line].wholeline + endline;
                ref_underline = create_pointing_lines(ref_tokenized[line].words[word].col + ref_inpos, ref_tokenized[line].words[word].col, ref_tokenized[line].words[word].col + refword.size());
                curline = std::string("    Current line, column ") + std::to_string(cur_tokenized[line].words[word].col + 1) + ":" + endline + "        "  + cur_tokenized[line].wholeline + endline;
                cur_underline = create_pointing_lines(cur_tokenized[line].words[word].col + cur_inpos, cur_tokenized[line].words[word].col, cur_tokenized[line].words[word].col + curword.size());
                result = false;
                break;
            }
        }
        if(!result) 
            break;
    }
    if(!result)
        out_stream << main_error_line << refline << ref_underline << curline << cur_underline;
    return result;
}

bool check_listing_at_pass(loops::Func func, std::string& errmessage,
                           const std::string& passname, const std::string& filename, bool tolerable_defect)
{
    std::stringstream errstream;

    bool result = true;
    bool rewrite = false;
    std::string curtext;
    ::std::ostringstream curstream(curtext, ::std::ios::out);
    std::string errprefix;
    if (passname == "CP_IR_TO_ASSEMBLY")
    {
        func.printAssembly(curstream);
        const std::string tarcname = ((FuncImpl*)getImpl(&func))->getContext()->getPlatformName();
        errprefix = std::string("    Check ") + tarcname + " assembly listing is failed: ";
    }
    else
    {
        errprefix = std::string("    Check of intermediate representation at pass ") + passname + "is failed: ";
        func.printIR(curstream, passname);
    }
    curtext = curstream.str();
    if(std::filesystem::exists(filename))
    {
        std::ifstream refstream(filename.c_str(), ::std::ios::in);
        if(!refstream.good())
        {
            errstream << errprefix + "cannot open file : \"" << filename << "\"." << std::endl;
            rewrite = RECREATE_REFERENCE_TEXTS;
        }
        else
        {
            std::string reftext((std::istreambuf_iterator<char>(refstream)), std::istreambuf_iterator<char>());
            if(!check_listing_equality(curtext, reftext, errprefix, errstream, tolerable_defect, func.name(), passname))
            {
                rewrite = RECREATE_REFERENCE_TEXTS;
                result = false;
            }
        }
    }
    else
    {
        errstream << errprefix + "file doesn't exist: \""<<filename<<"\"."<<std::endl;
        rewrite = RECREATE_REFERENCE_TEXTS;
        result = false;
    }
    if(rewrite)
    {
        std::ofstream rwrtstream(filename.c_str(), ::std::ios::out);
        if(!rwrtstream.good())
            errstream<< errprefix + "cannot open file for rewrite: \""<<filename<<"\"."<<std::endl;
        rwrtstream<<curtext;
        rwrtstream.close();
    }
    errmessage = errstream.str();
    return result;
}

bool intermediate_representation_is_stable(loops::Func func, std::string& errmessage, bool tolerable_defect)
{
    std::vector<std::string> passes_names = loops::Context().get_all_passes();
    passes_names.pop_back();//We are not considering binaries. 
    passes_names.pop_back();//And we are not considering assembly in this function. 
    const std::string func_name = func.name();
    bool result = true;
    for (int passn = 0; passn < passes_names.size(); passn++)
    {
        std::string filename = LISTINGS_ROOT + std::to_string(passn + 1) + "_" + passes_names[passn] + "/" + func_name + ".tst";
        std::stringstream errstream;
        std::string interm_message;
        bool success = check_listing_at_pass(func, interm_message, passes_names[passn], filename, tolerable_defect);
        if (!success)
        {
            result = false;
            #if (RECREATE_REFERENCE_TEXTS == true)
                errmessage = interm_message + errmessage;
            #else 
                errmessage = interm_message;
                break;
            #endif 
        }
    }
    return result;
}

bool assembly_is_stable(loops::Func func, std::string& errmessage, bool tolerable_defect)
{
    std::vector<std::string> passes_names = loops::Context().get_all_passes();
    auto found = std::find(passes_names.begin(), passes_names.end(), "CP_IR_TO_ASSEMBLY");
    Assert(found >= passes_names.begin() && found < passes_names.end());
    int passn = (int)(found - passes_names.begin());
    std::string filename = LISTINGS_ROOT + std::to_string(passn + 1) + "_" + passes_names[passn] + "/" + func.name() + ".tst";
    return check_listing_at_pass(func, errmessage, passes_names[passn], filename, tolerable_defect);
}

bool zip_is_equal(const std::string& filename, miniz_cpp::zip_info& fil)
{
    bool res = std::filesystem::exists(filename) &&
        fil.file_size == std::filesystem::file_size(filename);
    if (res)
    {
        size_t fsize = std::filesystem::file_size(filename);
        std::vector<char> buf(fsize, 0);
        std::ifstream filstr(filename.c_str(), std::ios::in | std::ios::binary);
        filstr.read(buf.data(), fsize);
        uint32_t crc = mz_crc32(MZ_CRC32_INIT, (mz_uint8*)buf.data(), fsize);
        res = crc == fil.crc;
    }
    return res;
}

void unzip_listings()
{
    assert(std::filesystem::directory_entry(LOOPS_TEST_DIR"/refasm/").exists());
    //1.)Check and recreate folder system.
    {
        if(!std::filesystem::directory_entry(LISTINGS_ROOT).exists())
            std::filesystem::create_directory(LISTINGS_ROOT);
        std::vector<std::string> passes_names = loops::Context().get_all_passes();
        passes_names.pop_back();
        for (int passn = 0; passn < passes_names.size(); passn++)
            passes_names[passn] = std::to_string(passn + 1) + "_" + passes_names[passn];
        for(std::string passname : passes_names)
            if(!std::filesystem::directory_entry(LISTINGS_ROOT + passname).exists())
                std::filesystem::create_directory(LISTINGS_ROOT + passname);
    }
    //2.)Unpack all zip files. 
    const std::string zipname = std::string(LOOPS_TEST_DIR"/refasm/") + toLower(ARCHname()) + "_" + toLower(OSname()) + ".zip";
    if (!std::filesystem::exists(zipname))
    {
        std::cerr << "WARNING: There is no listing archive. Listings can be non-actual. Recreate listing by setting RECREATE_REFERENCE_TEXTS to true." << std::endl;
        return;
    }

    miniz_cpp::zip_file file(zipname);
    auto files2unpack = file.infolist();
    for (miniz_cpp::zip_info& fil : files2unpack)
        if(
#if (RECREATE_TOLERABLE_DEFECT == true)
            fil.filename != "defect.bin" && 
#endif
            !zip_is_equal(LISTINGS_ROOT + fil.filename, fil))
            file.extract(fil.filename, LISTINGS_ROOT);

    //3.)Read defect file
    std::string defect_file = LISTINGS_ROOT"defect.bin";
    if (std::filesystem::exists(zipname))
    {
        std::ifstream defectstr(defect_file.c_str(), std::ios::in | std::ios::binary);
        size_t defect_sz;
        defectstr.read((char*)&defect_sz, sizeof(defect_sz));
        while(defect_sz--)
            defects.insert(listing_defect::read(defectstr));
    }
}

void refresh_zip_listings()
{
#if (RECREATE_TOLERABLE_DEFECT == true)
    std::string defect_file = LISTINGS_ROOT"defect.bin";
    {
        std::ofstream defectstr(defect_file.c_str(), std::ios::out | std::ios::binary);
        size_t defect_sz = defects.size();
        defectstr.write((char*)&defect_sz, sizeof(defect_sz));
        for (const listing_defect& defect : defects)
            defect.write(defectstr);
    }
#endif

#if (RECREATE_REFERENCE_TEXTS == true)
    const std::string zipname = std::string(LOOPS_TEST_DIR"/refasm/") + toLower(ARCHname()) + "_" + toLower(OSname()) + ".zip";
    miniz_cpp::zip_file file;
    std::vector<std::string> passes_names = loops::Context().get_all_passes();
    passes_names.pop_back();
    for (int passn = 0; passn < passes_names.size(); passn++)
        passes_names[passn] = std::to_string(passn + 1) + "_" + passes_names[passn];
    for (std::string passname : passes_names)
    {
        std::filesystem::directory_iterator pass_dir(LISTINGS_ROOT + passname);
        for (auto fil : pass_dir)
            if (fil.path().filename().extension().string() == ".tst")
                file.write(LISTINGS_ROOT + passname + "/" + fil.path().filename().string(), passname + "/" + fil.path().filename().string());
    }
    #if (RECREATE_TOLERABLE_DEFECT == true)
        file.write(defect_file, "defect.bin");
    #endif
    file.save(zipname);
#endif
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
