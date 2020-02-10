public class Test6 {
	public static void printInt(int value) {
		System.out.println(value);
	}

	public static void main(String[] args) {
        int a = 1;
        int b = 2;
        int c = 3;
        int d = 4;
        int e = 5;
        int x = 0;

outer:
        for (int i = 0; i < d; i++) {
            if (x > 20) {
                break;
            }
            for (int j = 0; j < e; j++) {
                if (i < j) {
                    continue outer;
                }else if (i == j) {
                    continue;
                }
                x = a * x + b;
            }
        }

		printInt(x);
	}

}
