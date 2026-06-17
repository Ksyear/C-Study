#include <iostream>

double adc_to_volts(int raw, double vref=3.3);
void print_channel(int ch, double volts);

int main()
{
    print_channel(1, adc_to_volts(512));

    print_channel(2, adc_to_volts(256));    
    
    print_channel(3, adc_to_volts(1023));    
}

double adc_to_volts(int raw, double vref)
{
    vref = raw * vref / 1023.0;
    return vref;
}

void print_channel(int ch, double volts)
{
    std::cout << "ch"<< ch << ": " << volts << " V\n";
}
