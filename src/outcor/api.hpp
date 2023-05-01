class iSink {
public:
   virtual void write(time_t ts, const std::string& s) = 0;
};

class iOutCorrelator {
public:
   virtual iSink& getOut() = 0;
   virtual iSink& getErr() = 0;
};
