#ifndef ___ledit_printer___
#define ___ledit_printer___

namespace cui { class iStyler; }
namespace pen { class object; }

namespace ledit {

class cmdLineState;

class printer {
public:
   printer(pen::object& p, cui::iStyler& s) : m_pen(p), m_style(s) {}

   void initialize(cmdLineState& s);
   void print(cmdLineState& s);
   void updateHelp(cmdLineState& s, const std::string& newHelp);

private:
   pen::object& m_pen;
   cui::iStyler& m_style;
};

} // namespace ledit

#endif // ___ledit_printer___
