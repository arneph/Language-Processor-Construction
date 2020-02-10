public class Test7 {
	public static void printInt(int value) {
		System.out.println(value);
	}

	public static void main(String[] args) {
        int a = 2;
        int b = 5;
        int c = 0;
        
        if (a < b) {
            while (a < b) {
                if (c >= 42) {
                    break;
                }
                c++;
                if (c > 19) {
                    continue;
                }
                c++;
                if (c > 11) {
                    continue;
                }
                c++;
                if (c > 7) {
                    continue;
                }
            }
        }else{
        outer:
            while (c < 10) {
                for (int i = 0; i < 20; i++) {
                    c += i;
                    if (i < 10) {
                        continue outer;
                    }
                    if (i < 15) {
                        continue;
                    }
                }
            }
        }

		printInt(c);
	}

}
