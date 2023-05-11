#define WIN32_LEAN_AND_MEAN
#include "../cmn/autoPtr.hpp"
#include "../cmn/service.hpp"
#include "../cui/api.hpp"
#include "../cui/pen.hpp"
#include "../tcatlib/api.hpp"
#include "api.hpp"
#include <sstream>
#include <windows.h>

namespace outcor {

class defStdoutColorer : public iOutputColorer {
public:
   virtual std::string color(const std::string& blob)
   {
      return blob;
   }
};

class defStderrColorer : public iOutputColorer {
public:
   virtual std::string color(const std::string& blob)
   {
      std::stringstream s;
      s << pen::fgcol(pen::kRed,true) << blob << pen::fgcol(pen::kDefault);
      return s.str();
   }
};

class colorerBase : public iOutputColorer {
protected:
   bool startsWith(const std::string& pattern, const char *pThumb)
   { return ::strncmp(pattern.c_str(),pThumb,pattern.length()) == 0; }
};

class gitStdoutColorer : public colorerBase {
public:
   gitStdoutColorer() : m_state(0), m_color(pen::kDefault) {}

   virtual std::string color(const std::string& blob)
   {
      std::stringstream out;
      const char *pThumb = blob.c_str();

      for(;*pThumb!=0;pThumb++)
      {
         if(startsWith("Changes to be committed:",pThumb))
         {
            m_color = pen::kGreen;
         }
         else if(startsWith("Changes not staged for commit:",pThumb))
         {
            m_color = pen::kRed;
         }
         else if(startsWith("Untracked files",pThumb))
         {
            m_color = pen::kRed;
         }
         else if(match(0,"\t",pThumb))
         {
            // insert color
            out << pen::fgcol(m_color,true);
            m_state = 1;
         }
         else if(match(1,"\n",pThumb))
         {
            // disable color
            out << pen::fgcol(pen::kDefault);
            m_state = 0;
         }

         out << std::string(pThumb,1);
      }

      return out.str();
   }

private:
   bool match(size_t s, const std::string& pattern, const char *pThumb)
   { return m_state == s && startsWith(pattern,pThumb); }

   size_t m_state;
   pen::colors m_color;
};

class makeStdoutColorer : public colorerBase {
public:
   virtual std::string color(const std::string& blob)
   {
      std::stringstream out;
      const char *pThumb = blob.c_str();

      for(;*pThumb!=0;)
      {
         if(startsWith("-->",pThumb))
         {
            out
               << pen::fgcol(pen::kYellow,true)
               << "-->"
               << pen::fgcol(pen::kDefault);
            pThumb += 3;
         }
         else if(startsWith("*** FAILED ***",pThumb))
         {
            out
               << pen::fgcol(pen::kYellow,true) << pen::bgcol(pen::kRed)
               << "*** FAILED ***"
               << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault);
            pThumb += 14;
         }
         else if(startsWith("test FAILED",pThumb))
         {
            out
               << pen::fgcol(pen::kRed,true)
               << "test FAILED"
               << pen::fgcol(pen::kDefault);
            pThumb += 11;
         }
         else if(startsWith("result: pass",pThumb))
         {
            out
               << "result: "
               << pen::fgcol(pen::kYellow,true) << pen::bgcol(pen::kBlue)
               << "pass"
               << pen::fgcol(pen::kDefault) << pen::bgcol(pen::kDefault);
            pThumb += 12;
         }
         else
            out << std::string(pThumb++,1);
      }

      return out.str();
   }
};

class makeStderrColorer : public colorerBase {
public:
   virtual std::string color(const std::string& blob)
   {
      std::stringstream out;
      out << pen::fgcol(pen::kRed,true);

      const char *pThumb = blob.c_str();

      for(;*pThumb!=0;)
      {
         if(startsWith("\xe2\x80\x98",pThumb)) // open single-quote
         {
            out << "'";
            pThumb += 3;
         }
         else if(startsWith("\xe2\x80\x99",pThumb)) // close single-quote
         {
            out << "'";
            pThumb += 3;
         }
         else if(startsWith(" error: ",pThumb))
         {
            out
               << " "
               << pen::fgcol(pen::kYellow,true)
               << pen::bgcol(pen::kRed)
               << "error:"
               << pen::fgcol(pen::kRed,true)
               << pen::bgcol(pen::kDefault)
               << " ";
            pThumb += 8;
         }
         else if(startsWith(" warning: ",pThumb))
         {
            out
               << " "
               << pen::fgcol(pen::kBlue,true)
               << pen::bgcol(pen::kRed)
               << "warning:"
               << pen::fgcol(pen::kRed,true)
               << pen::bgcol(pen::kDefault)
               << " ";
            pThumb += 10;
         }
         else if(startsWith(" In member function ",pThumb))
         {
            out
               << " "
               << pen::fgcol(pen::kBlue,true)
               << "In member function"
               << pen::fgcol(pen::kRed,true)
               << " ";
            pThumb += 20;
         }
         else
            out << std::string(pThumb++,1);
      }

      this does not compile

      out << pen::fgcol(pen::kDefault);
      return out.str();
   }
};

class outputColorerFactory : public iOutputColorerFactory {
public:
   virtual iOutputColorer& create(const std::string& resolvedPath, bool isOut)
   {
      if(resolvedPath == "git status" && isOut)
         return *new gitStdoutColorer();

      else if(resolvedPath == "make" && isOut)
         return *new makeStdoutColorer();
      else if(resolvedPath == "make" && !isOut)
         return *new makeStderrColorer();

      if(isOut)
         return *new defStdoutColorer();
      else
         return *new defStderrColorer();
   }
};

tcatExposeTypeAs(outputColorerFactory,iOutputColorerFactory);

} // namespace outcor
