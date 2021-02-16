
class Topic
{
private:
    char* name;
    unsigned int length;
    unsigned int maxLenght = 120;
    char value[120];
public:
    Topic(char* name);
    char* getValue();
    void setValue(byte* value, unsigned int length);
    char* getName();
    unsigned int getLength();
};

Topic::Topic(char* name){
  this->name = name;
  length = 0;
}

char* Topic::getValue(){
  return value;
}

unsigned int Topic::getLength(){
  return length;
}

void Topic::setValue(byte* value, unsigned int length){
  this->length = length <= maxLenght ? length : maxLenght;

  for (int i = 0; i < this->length ; i++) {
    this->value[i] = value[i];
  }
}

char* Topic::getName(){
  return name;
}