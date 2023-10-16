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

namespace loops
{

inline bool fileexists(const std::string& name) //TODO(ch): use GetFileAttributes for windows
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

inline std::string toLower(const std::string& tL)
{
    std::string res = tL;
    std::transform(res.begin(), res.end(), res.begin(), [](char t) {return (char)::tolower(t); });
    return res;
}

bool Test::testAssembly(const std::string& a_fixtureName, bool a_rewriteIfWrong)
{
    loops::Func func = CTX.getFunc(a_fixtureName);
    std::string tarcname = CTX.getPlatformName();
    std::string arcOSsuffix = toLower(tarcname);
    std::string bfilename(LOOPS_TEST_DIR"/refasm/");
    arcOSsuffix += std::string("/") + toLower(OSname());
    std::string tfilename = bfilename;
    bool result = true;
    { //Bytecode check
        bool rewrite = false;
        std::string bctext;
        ::std::ostringstream bcstream(bctext, ::std::ios::out);
        func.printBytecode(bcstream);
        bctext = bcstream.str();
        bfilename += arcOSsuffix + "/bytecode/";
        bfilename += a_fixtureName + ".tst";
        if(fileexists(bfilename))
        {
            std::ifstream refstream(bfilename.c_str(), ::std::ios::in);
            if(!refstream.good())
                (*m_out)<<"    Bytecode check failed: cannot open file: \""<<bfilename<<"\"."<<std::endl;
            std::string bcref((std::istreambuf_iterator<char>(refstream)),
                             std::istreambuf_iterator<char>());
            if(!checkListingEquality(bctext, bcref, "Bytecode check failed"))
            {
                if(a_rewriteIfWrong)
                    rewrite = a_rewriteIfWrong;
                result = false;
            }
        }
        else
        {
            (*m_out)<<"    Bytecode check failed: file doesn't exist: \""<<bfilename<<"\"."<<std::endl;
            if(a_rewriteIfWrong)
                rewrite = a_rewriteIfWrong;
            result = false;
        }
        if(rewrite)
        {
            std::ofstream rwrtstream(bfilename.c_str(), ::std::ios::out);
            if(!rwrtstream.good())
                (*m_out)<<"    Bytecode check failed: cannot open file for rewrite: \""<<bfilename<<"\"."<<std::endl;
            rwrtstream<<bctext;
            rwrtstream.close();
        }
    }
    if (result || a_rewriteIfWrong) //Target assembly check
    {
        bool rewrite = false;
        std::string tatext;
        ::std::ostringstream tastream(tatext, ::std::ios::out);
        func.printAssembly(tastream);
        tatext = tastream.str();
        tfilename += arcOSsuffix + "/";
        tfilename += a_fixtureName + ".tst";
        if(fileexists(tfilename))
        {
            std::ifstream refstream(tfilename.c_str(), ::std::ios::in);
            if(!refstream.good())
                (*m_out)<<"    Check for "<<tarcname<<" failed: cannot open file: \""<<tfilename<<"\"."<<std::endl;

            
            std::string bcref((std::istreambuf_iterator<char>(refstream)),
                             std::istreambuf_iterator<char>());
            if(!checkListingEquality(tatext, bcref, "Check for "+tarcname+" failed"))
            {
                if(a_rewriteIfWrong)
                    rewrite = a_rewriteIfWrong;
                result = false;
            }
        }
        else
        {
            (*m_out)<<"    Check for "<<tarcname<<" failed: file doesn't exist: \""<<tfilename<<"\"."<<std::endl;
            if(a_rewriteIfWrong)
                rewrite = a_rewriteIfWrong;
            result = false;
        }
        if(rewrite)
        {
            std::ofstream rwrtstream(tfilename.c_str(), ::std::ios::out);
            if(!rwrtstream.good())
                (*m_out)<<"    Check for "<<tarcname<<" failed: cannot open file for rewrite: \""<<tfilename<<"\"."<<std::endl;
            rwrtstream<<tatext;
            rwrtstream.close();
        }
    }
    return result;
}
std::string Test::OSname()
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

bool Test::checkListingEquality(const std::string& curL, const std::string& refL, const std::string& errMes)
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
        (*m_out)<<mainErrorMes.c_str()<<std::endl;
        (*m_out)<<"        Row: " << rowNum<< " | Col: "<<colNum<<std::endl;
        size_t curErrEndPos = curL.find_first_of('\n', curPos); //TODO(ch): What about windows style endlines?
        size_t refErrEndPos = refL.find_first_of('\n', refPos);
        colNum-=1;
        curPos -= colNum;
        refPos -= colNum;
        std::string curProblemLine = curL.substr(curPos, curErrEndPos - curPos);
        std::string refProblemLine = refL.substr(refPos, refErrEndPos - refPos);
        (*m_out)<<"        Current listing line: \""<<curProblemLine<<"\""<<std::endl;
        (*m_out)<<"        Reference line      : \""<<refProblemLine<<"\""<<std::endl;
    }
    return result;
}

std::shared_ptr<TestSuite> TSSingleton;
TestSuite* TestSuite::getInstance()
{
    if(TSSingleton.get() == nullptr)
        TSSingleton.reset(new TestSuite());
    return TSSingleton.get();
}

void TestSuite::run(bool rewriteListings)
{
    size_t total = 0;
    size_t success = 0;
    size_t failed = 0;
    size_t executionFail = 0;
    size_t listingFail = 0;
    for(std::shared_ptr<Test> tptr: m_testList)
        tptr->generateCode();
    CTX.compileAll();
    for(std::shared_ptr<Test> tptr: m_testList)
        for(std::string& fixtureName : tptr->fixturesNames())
        {
            ++total;
            (*m_out)<<"[TEST:\""<<fixtureName<<"\"]"<<std::endl;
            if(!tptr->testExecution(fixtureName))
            {
                ++failed;
                ++executionFail;
                (*m_out)<<"[FAIL]"<<std::endl;
                continue;
            }
            if(!tptr->testAssembly(fixtureName, rewriteListings))
            {
                ++failed;
                ++listingFail;
                (*m_out)<<"[FAIL]"<<std::endl;
                continue;
            }
            ++success;
            (*m_out)<<"[OK]"<<std::endl;
        }
    (*m_out)<<"Total tests       : "<<total<<std::endl;
    (*m_out)<<"Success           : "<<success<<std::endl;
    (*m_out)<<"Failed            : "<<failed<<std::endl;
    if(failed)
    {
        (*m_out)<<"Fails distibution : "<<std::endl;
        (*m_out)<<"Execution fails   : "<<executionFail<<std::endl;
        (*m_out)<<"Listing fails     : "<<listingFail<<std::endl;
    }
}

TestSuite::TestSuite(std::ostream& a_out) : CTX(), m_out(&a_out)
{
    CTX.debugModeOn();
    getImpl(&CTX)->getBackend()->switchOnSpillStressMode();
}
};

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
