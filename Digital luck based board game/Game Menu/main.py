import pygame
import button
import random

pygame.init()

#create game window
SCREEN_WIDTH = 800   #1.6x Orig: 800
SCREEN_HEIGHT = 600  #1.2x Orig: 600

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Main Menu")

#game variables
game_play = True
menu_state = "main"

#define fonts
font = pygame.font.SysFont("arialblack", 40)

#define colours
TEXT_COL = (255, 255, 255)

#load button images
play_img = pygame.image.load("images/button_resume.png").convert_alpha()
options_img = pygame.image.load("images/button_options.png").convert_alpha()
quit_img = pygame.image.load("images/button_quit.png").convert_alpha()
video_img = pygame.image.load('images/button_video.png').convert_alpha()
audio_img = pygame.image.load('images/button_audio.png').convert_alpha()
keys_img = pygame.image.load('images/button_keys.png').convert_alpha()
back_img = pygame.image.load('images/button_back.png').convert_alpha()

#create button instances
play_button = button.Button(304, 125, play_img, 1)          #304, 125
options_button = button.Button(297, 250, options_img, 1)    #297, 250
quit_button = button.Button(336, 375, quit_img, 1)          #336, 375
video_button = button.Button(226, 75, video_img, 1)         #226, 75
audio_button = button.Button(225, 200, audio_img, 1)        #225, 200
keys_button = button.Button(246, 325, keys_img, 1)          #246, 325
back_button = button.Button(332, 450, back_img, 1)          #332, 450

def draw_text(text, font, text_col, x, y):
  img = font.render(text, True, text_col)
  screen.blit(img, (x, y))

#Game Funtions
def left(pos, players):
    return ((pos + players - 1) % players)

def right(pos, players):
  return ((pos + 1) % players)

#game loop
run = True
while run:

  screen.fill((52, 78, 91))

  #check if game is paused
  if game_play == False:
    #check menu state
    if menu_state == "main":
      #draw pause screen buttons
      if play_button.draw(screen):
        game_play = True

        PLAYERS = 14
        MONEY = 3

        bots = ['Bot John', 'Bot Sam', 'Bot Ken', 'Bot Ross', 'Bot Rose', 'Bot Ann', 'Bot Katy', 'Bot Mary', 'Bot Carl',
                'Bot Brent', 'Bot Lynn', 'Bot Janet', 'Bot Gary']
        dice_faces = ['LEFT', 'RIGHT', 'CENTER', 'PASS', 'PASS', 'PASS']

        '''
        '''
        game_main()
        def game_main():
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




      if options_button.draw(screen):
        menu_state = "options"
      if quit_button.draw(screen):
        run = False
    #check if the options menu is open
    if menu_state == "options":
      #draw the different options buttons
      if video_button.draw(screen):
        print("Video Settings")
      if audio_button.draw(screen):
        print("Audio Settings")
      if keys_button.draw(screen):
        print("Change Key Bindings")
      if back_button.draw(screen):
        menu_state = "main"
  else:
    draw_text("Press SPACE to pause", font, TEXT_COL, 160, 250)

  #event handler
  for event in pygame.event.get():
    if event.type == pygame.KEYDOWN:
      if event.key == pygame.K_SPACE:
        game_play = False
    if event.type == pygame.QUIT:
      run = False

  pygame.display.update()

pygame.quit()