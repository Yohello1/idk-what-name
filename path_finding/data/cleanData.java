import java.io.*;
import java.util.*;

public class cleanData
{
    public static void main(String[] args) throws FileNotFoundException
    {
        System.out.println("Going to print clean the data for this file: " + args[0]);

        File myObj = new File(args[0]);
        PrintWriter output = new PrintWriter("cleanedData.txt");
        Scanner myReader = new Scanner(myObj);

        while (myReader.hasNextLine())
        {
            String data = myReader.nextLine();
            int count = 0;

            for (int i = 0; i < data.length(); i++)
            {
                if (data.charAt(i) == ',')
                {
                    count++;
                }
            }

            if(count == 1)
            {
                output.print(data);
                output.print('\n');
            }

        }
        output.close();
        myReader.close();

    }

}
