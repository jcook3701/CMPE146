#ifndef LABUART_H
#define LABUART_H

class LabUART
{
private:
public:
    LabUART();
    ~LabUART();
  
    // TODO: Fill in methods for init(), transmit(), receive() etc.
	void init(void);
	void transmit(char c);
	char receive(void);
  
    // Optional: For the adventurous types, you may inherit from "CharDev" class to get a lot of funcionality for free
};

#endif
