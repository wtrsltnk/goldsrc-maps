#ifndef BSPPARSER_H
#define BSPPARSER_H

#include <vector>

class BspParser
{
    unsigned char *_data;
    int _size;
public:
    BspParser(unsigned char *data, int size);

    bool LoadScene(class BspScene* scene);
};

#endif // BSPPARSER_H
