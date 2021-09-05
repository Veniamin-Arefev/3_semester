
int
bitcount(STYPE value)
{
    int counter = 0;
    UTYPE myvalue = (UTYPE) value;
    while(myvalue){
        if(myvalue & 1){
            counter++;
        }
        myvalue = myvalue >> 1;
    }
    return counter;
}

