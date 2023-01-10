import random

def left(pos, players):
    return ((pos + players - 1) % players)

def right(pos, players):
    return ((pos + 1) % players)

def instructions():
    print('\nThis is a simple imaginary game in which the user inputs values for the “number of players” and “random seed” as prompted by the program.') 
    print('The program checks to make sure that these values are valid and abide by the rules of the game. The game can have between 2-14 players who')
    print('all start off with a bank of $3 and take turns rolling dice with up to 3 dice being rolled at a time for players possessing $3 or more. the')
    print('number of dice allowed to roll is equivalent to the player’s current cash value if below $3. Those players who do not have any money will not')
    print('roll in this case. Once a player rolls, there are 4 outcomes based on a 6 face die as the 3 dice have custom values. One of the die’s faces ')
    print('is labeled with “L” pertaining to “Left”, another is labeled with “R” pertaining to “Right”, another is labeled “C” pertaining to “Center” ')
    print('while the remaining 3 faces are labeled with “•” pertaining to “Pass”. As the player rolls the dice, they either pass and don’t do anything')
    print('or pay $1 towards what is labeled on the dice. If a player rolls a “L”, they will pay $1 to the player on their left, an “R” will make them')
    print('"pay" $1 to the person to the right and a “C” will make them deposit their $1 in the “center pot”. The players will continue taking turns ')
    print('rolling based off the value of their bank clockwise to the "right" until there is only 1 player with any money remaining. The last remaining ')
    print('player with money will then win the game and take all the money in the pot.')
    back = input("\n**Press B to go back to main menu**")
    if back == "b" or back == "B":
        main()

PLAYERS = 14
MONEY = 3

bots = ['Bot John', 'Bot Sam', 'Bot Ken', 'Bot Ross', 'Bot Rose', 'Bot Ann', 'Bot Katy', 'Bot Mary', 'Bot Carl', 'Bot Brent', 'Bot Lynn', 'Bot Janet', 'Bot Gary']
dice_faces = ['LEFT', 'RIGHT', 'CENTER', 'PASS', 'PASS', 'PASS']

def main():
    money = [MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY, MONEY]
    players = PLAYERS
    seed = 2125
    centercash = 0
    menu = input("**Type I for game instructions or press ENTER to play**")
    if menu == "i" or menu == "I":
        instructions()
    seed = int(input("Random seed: "))
    if seed < 1:
        print("Invalid random seed.")
        exit()
    players = int(input("How many players?: "))
    if players < 2 or players > PLAYERS:
        print("This game can only be played with 2-14 players!")
        exit()
    playername = input("What is your name?: ")
    bots.insert(0, playername)
    random.seed(seed)
    playing = players
    while playing > 1:
        for p in range(players):
            if money[p] == 0:
                continue
            rolls = money[p]
            if rolls > 3:
                rolls = 3
            print(bots[p], "rolls...")
            for r in range(rolls):
                roll = random.randint(1, 6) - 1
                if dice_faces[roll] == "LEFT":
                    leftplr = left(p, players)
                    print(" gives $1 to", bots[leftplr])
                    money[p] -= 1
                    if money[p] == 0:
                        playing -= 1
                    if money[leftplr] == 0:
                        playing += 1
                    money[leftplr] += 1
                if dice_faces[roll] == 'RIGHT':
                    rightplr = right(p, players)
                    print(" gives $1 to", bots[rightplr])
                    money[p] -= 1
                    if money[p] == 0:
                        playing -= 1
                    if money[rightplr] == 0:
                        playing += 1
                    money[rightplr] += 1
                if dice_faces[roll] == 'CENTER':
                    rightplr = right(p, players)
                    print(" puts $1 in the pot")
                    money[p] -= 1
                    if money[p] == 0:
                        playing -= 1
                    centercash += 1
                if dice_faces[roll] == 'PASS':
                    print(" gets a pass")
            print('\n')
    for i in range(players):
        if money[i] > 0:
            dollar_money = str(money[i])
            dollar_money = "${:}".format(dollar_money)
            dollar_center = str(centercash)
            dollar_center = "${:}".format(dollar_center)
            print(bots[i], "wins the", dollar_center, "pot with", dollar_money, "left in the bank!")
            exit()
                    
if __name__ == '__main__':
    main()