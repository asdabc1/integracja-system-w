#ifndef INDEXASSIGNERS_H
#define INDEXASSIGNERS_H

class eurIndexAssigner {
public:
    int operator[](double val) {
        if (val >= 3.90 && val < 4)
            return 0;
        else if (val < 4.1)
            return 1;
        else if (val < 4.2)
            return 2;
        else if (val < 4.3)
            return 3;
        else if (val < 4.4)
            return 4;
        else if (val < 4.5)
            return 5;
        else if (val < 4.6)
            return 6;
        else if (val < 4.7)
            return 7;
        else if (val < 4.8)
            return 8;
        else if (val < 4.9)
            return 9;
    }
};

class usdIndexAssigner {
public:
    int operator[](double val) {
        if (val >= 3.0 && val < 3.1)
            return 0;
        else if (val < 3.2)
            return 1;
        else if (val < 3.3)
            return 2;
        else if (val < 3.4)
            return 3;
        else if (val < 3.5)
            return 4;
        else if (val < 3.6)
            return 5;
        else if (val < 3.7)
            return 6;
        else if (val < 3.8)
            return 7;
        else if (val < 3.9)
            return 8;
        else if (val < 4)
            return 9;
        else if (val < 4.1)
            return 10;
        else if (val < 4.2)
            return 11;
        else if (val < 4.3)
            return 12;
        else if (val < 4.4)
            return 13;
        else if (val < 4.5)
            return 14;
        else if (val < 4.6)
            return 15;
        else if (val < 4.7)
            return 16;
        else if (val < 4.8)
            return 17;
        else if (val < 4.9)
            return 18;
        else if (val < 5.1)
            return 19;
    }
};

#endif //INDEXASSIGNERS_H
