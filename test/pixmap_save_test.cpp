#include "render.hpp"
#include <cmath>

void minitest()
{
    rt::Pixmap px(2,2);
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            printf("%p: %p %p %p\n", &(px(i,j)), &(px(i,j).red), &(px(i,j).green), &(px(i,j).blue));
        }
    }
}

int main()
{
    rt::Pixmap px(500, 500);

    for (int i = 0; i < 500; ++i)
    {
        for (int j = 0; j < 500; ++j)
        {
            px(i,j) = rt::RGB(0, ((1000 - (i+j))*(255.0/1000.0)), ((i+j)*(255.0/1000.0)));
        }
    }
    px.saveFile("kekw.png", rt::Pixmap::format::png);

    return 0;
}