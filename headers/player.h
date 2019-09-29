#ifndef PLAYER_H
#define PLAYER_H
class Player{
    private:
        Sprite sprite;
        Sprite turret;
    public:
        Player(Sprite sprite, Sprite turret, int x, int y); //constructor, initialize the x, y, and sprite
        int x, y, lives; //x and y coordinates, health points left
        Weapon weapon; //equipped weapon
        bool Fire(); //return true if the player fired successfully
        bool Rotate(float angle); //rotate the object
        bool RotateTurret(float angle); //rotate the turret
        bool Move(int x, int y); //move x offset from current x and y offset from current y
        bool Place(int x, int y); //place/teleport to an x and y
        bool Draw();
        ~Player();
};  
#endif
