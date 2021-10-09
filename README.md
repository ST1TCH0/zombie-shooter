# Online Co-op Zombie Shooter
This game is made as a group university project. The main goal was to set connection between computers in one local network and use it. The game is designed in such a way that one of players is a server and others are clients. 

The project is made in C++ with SFML. To connect players we added ``` winsock.h ``` library.

## How to Launch
1. Download Release folder.
2. Run ```ShooterClient.exe``` to launch the game.
3. Host has to click the "HOST" button and clients have to enter host IP before clicking the "CLIENT" button.
4. Enjoy :wink:

<p float="left">
<img src="https://user-images.githubusercontent.com/64905551/136289216-82c82e0b-79be-4889-b712-a9791a8e7905.png" width="480" />
<img src="https://user-images.githubusercontent.com/64905551/136289252-5b63391d-c84b-478d-8613-5f7c96ffd2b2.png" width="480" />
</p>

## Gameplay

After game started you can see all players with their nicknames and health bars at the center of the map. Players use WASD to move and mouse to aim and shoot. In top left corner there are kill score and FPS counter.

<p align="center">
  <img class="image" src="https://user-images.githubusercontent.com/64905551/136290662-89fc4786-02d0-45d3-92d9-2e15c5586ac7.gif" width="700" margin-left="100"/>
</p>

When player loses all HP his nickname's color changes to red.

<p align="center">
  <img class="image" src="https://user-images.githubusercontent.com/64905551/136290324-d83cf778-3399-4f4c-9610-4d9ceb03e92c.png" width="700" margin-left="100"/>
</p>

## To do
As we implemented netcode from scratch it is not optimized well. Ping influences significantly on players movement. 

Possible code improvements:
1. Movement interpolation.
2. Optimizing exchanging data packagings.
