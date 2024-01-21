// java, make sure to run with -ea option

import java.util.*;
import java.io.*;

class Main
{
    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);

        long currentLong = sc.nextLong();
        asserts(1 < currentLong && currentLong < 100001);

        currentLong = sc.nextLong();
        assert( 0 < currentLong && currentLong < 1000001);

        currentLong = sc.nextLong();
        assert( 0 < currentLong && currentLong < 1000001);

        currentLong = sc.nextLong();
        long compareTemp = 20000000000001L;
        assert(Math.abs(currentLong) < compareTemp);

        currentLong = sc.nextLong();
        ssert(Math.abs(currentLong) < compareTemp);
    }
}