int main() {
    int arr = 0;
    int sum = 0;
    int i = 0;
    
    for (i = 0; i < 10; i++) {
        if (arr >= 0) {
            sum += arr;
            
            if (sum > 100) {
                sum = 100;
            }
        } else {
            sum -= arr;
        }
    }
    
    while (sum > 0) {
        sum--;
        
        if (sum == 50) {
            do {
                sum = sum / 2;
            } while (sum > 25);
        }
    }
    
    return sum;
}
