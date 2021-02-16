#include <Arduino.h>

class AnalogSensor
{
private:
    int pin;
    int min;
    int max;
    int value;
public:
    AnalogSensor();
    AnalogSensor(int pin, int min, int max);
    double getValue();
    void setPin(int pin);
    void setMin(int min);
    void setMax(int max);
    void setValues(int pin, int min, int max);
};

AnalogSensor::AnalogSensor(){
};

AnalogSensor::AnalogSensor(int pin, int min, int max){
    this->setValues(pin, min, max);
}

void AnalogSensor::setValues(int pin, int min, int max){
    this->pin = pin;
    this->min = min;
    this->max = max;
}

void AnalogSensor::setPin(int pin){
    this->pin = pin;
};

void AnalogSensor::setMin(int min){
    this->min = min;
};

void AnalogSensor::setMax(int max){
    this->max = max;
};

/**
 *  vrací hodnotu jako procento v rozpětí min-max
 */ 
double AnalogSensor::getValue(){
    value = analogRead(pin);
    value = (value > max) ? max : value;
    value = (value < min) ? min : value;

    return 1 - (value - min)/(double)(max - min);
};