/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "tests.hpp"
#include "../src/common.hpp"        //TODO(ch): .. in path is bad practice. Configure project
#include "../src/reg_allocator.hpp" //TODO(ch): .. in path is bad practice. Configure project
#include <fstream>
#include <sstream>
#include <sys/stat.h> //TODO(ch): *nix-only.

namespace loops
{

inline bool fileexists(const std::string& name) //TODO(ch): need crossplatform solution
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

bool Test::testAssembly(bool a_rewriteIfWrong)
{
    std::string tarcname = CTX.getPlatformName();
    std::string arcOSsuffix = tarcname;
    std::string bfilename(LOOPS_TEST_DIR"/refasm/");
    if(tarcname == "Intel64")
        arcOSsuffix += std::string("/") + OSname();
    std::string tfilename = bfilename;
    bool result = true;
    { //Bytecode check
        bool rewrite = false;
        std::string bctext;
        ::std::ostringstream bcstream(bctext, ::std::ios::out);
        m_func.printBytecode(bcstream);
        bctext = bcstream.str();
        bfilename += arcOSsuffix + "/bytecode/";
        bfilename += m_func.name() + ".tst";
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
        m_func.printAssembly(tastream);
        tatext = tastream.str();
        tfilename += arcOSsuffix + "/";
        tfilename += m_func.name() + ".tst";
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
    {
        ++total;
        (*m_out)<<"[TEST:\""<<tptr->name()<<"\"]"<<std::endl;
        if(!tptr->testExecution())
        {
            ++failed;
            ++executionFail;
            (*m_out)<<"[FAIL]"<<std::endl;
            continue;
        }
        if(!tptr->testAssembly(rewriteListings))
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

TestSuite::TestSuite(std::ostream& a_out) : m_out(&a_out), CTX()
{
    getImpl(&CTX)->getBackend()->switchOnSpillStressMode();
}
};
