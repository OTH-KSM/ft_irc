

#include <iostream>

int test(){
    for(int i = 0; i < 10; i++)
    {
        std::cout << "before " << i << std::endl; 
        if (i % 2 == 0)
            continue;
        std::cout << "after " << i << std::endl;
    }
}
int main(){
    test();
}