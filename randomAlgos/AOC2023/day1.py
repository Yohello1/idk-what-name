sum = 0

with open("input.txt") as f:
    for line in f:
        # sum = 0
        #temp = max(line.rfind(i) for i in "1234567890")

        # if any(s in line for s in ('one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0')):
        #    print(s)
        #  print(temp)
        #sum += int(line[temp])
        # print(sum)

        # temp = line.find(next(filter(str.isdigit, line)))
        #sum += int(line[temp])*10

        lowest = 10000000000000
        highest = -1

        validNums = ['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']
        for x in validNums:
            temp = line.find(x)
            if temp != -1 and temp < lowest:

                lowest = temp

            temp = line.rfind(x)
            if temp != -1 and temp > highest:
                highest = temp

        if line[lowest].isdigit():
            #sum = 0
            sum += int(line[lowest])*10
            #print("hi")
#            print("lowest: " + line[lowest])
#            print(sum)
        else:
            if line[lowest] == 'o':
                sum += 10
            if line[lowest] == 't' and line[lowest+1] == 'w':
                sum += 20
            if line[lowest] == 't' and line[lowest+1] == 'h':
                sum += 30
            if line[lowest] == 'f' and line[lowest+1] == 'o':
                sum += 40
            if line[lowest] == 'f' and line[lowest+1] == 'i':
                sum += 50
            if line[lowest] == 's' and line[lowest+1] == 'i':
                sum += 60
            if line[lowest] == 's' and line[lowest+1] == 'e':
                sum += 70
            if line[lowest] == 'e':
                sum += 80
            if line[lowest] == 'n':
                sum += 90


        # pain du fromage
        lowest = highest
        if line[lowest].isdigit():
            sum += int(line[lowest])*1
        else:
            if line[lowest] == 'o':
                sum += 1
            if line[lowest] == 't' and line[lowest+1] == 'w':
                sum += 2
            if line[lowest] == 't' and line[lowest+1] == 'h':
                sum += 3
            if line[lowest] == 'f' and line[lowest+1] == 'o':
                sum += 4
            if line[lowest] == 'f' and line[lowest+1] == 'i':
                sum += 5
            if line[lowest] == 's' and line[lowest+1] == 'i':
                sum += 6
            if line[lowest] == 's' and line[lowest+1] == 'e':
                sum += 7
            if line[lowest] == 'e':
                sum += 8
            if line[lowest] == 'n':
                sum += 9
#        print(sum)

print(sum)
