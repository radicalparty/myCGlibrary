#include "convexhull3d/3dhull_function.cpp"
template<typename T>
void swap(T* a, T* b, T* c){
    T tmp = *a;
    *a = *b; *b = *c; *c = tmp;
}
int main() {
    std::cin.tie(); std::ios_base::sync_with_stdio(false);
    int a, b, c; std::cin >> a >> b >> c;
    swap(&a, &b, &c);
    std::cout << a << " " << b << " " << c;
    return 0;
}
