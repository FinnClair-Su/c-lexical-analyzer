int main() {
    int x = 5;
    int y = 10;
    int flag = 1;
    
    if (x > 5 && y < 10) {
        x = y;
    }
    
    if (x < 3 || y > 15) {
        y = x;
    }
    
    if (!flag) {
        flag = 1;
    }
    
    if (x == y) {
        return 1;
    }
    
    if (x != y) {
        return 0;
    }
    
    if (x <= 10 && y >= 5) {
        return x;
    }
    
    return 0;
}
