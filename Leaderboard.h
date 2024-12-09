//
// Created by Elizabeth Gonzalez on 11/28/24.
//

#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include <map>
#include <string>
#include <fstream>
using namespace std;
#pragma once


struct Player {
    string name;
    string score;

public:
    Player() : name(""), score("") {}
    Player(string name, string score) : name(name), score(score){}

    string getName(){ return name;}
    string getScore(){return score;}
};

class Leaderboard {
    string filename = "files/leaderboard.txt";
    int playerCount;
    vector<Player> players;
public:
    Leaderboard(): playerCount(0) {}
    Leaderboard(int playerCount) : playerCount(playerCount) {}
    vector<Player> getPlayers() {
        return players;
    }
    void read() {
        players.clear();
        string name, score;
        ifstream openFile(filename);
        while (getline(openFile, score, ',')) {
            getline(openFile, name);
            auto player = Player(name, score);
            players.push_back(player);
            playerCount++;
        }
    }
    void add(Player player) {
        players.push_back(player);
        playerCount++;
    }
    void remove() {
        players.pop_back();
    }
    void write() {
        ofstream openFile(filename);
        for (int i = 0; i < players.size(); i++) {
            if (players[i].getName().find('*') != players[i].getName().length()) {
                openFile << players[i].getScore() << "," <<
                    (players[i].getName() = players[i].getName().substr(0, players[i].getName().find('*'))) << endl;
            } else {
                openFile << players[i].getScore() << "," << players[i].getName() << endl;
            }
            playerCount++;
        }
    }
    void sortbyTime() {
        sort(players.begin(), players.end(), [](Player& a, Player& b) {
            string aTimeS = a.getScore();
            string newATimeS = aTimeS.substr(0, aTimeS.find(':')) +
                aTimeS.substr(aTimeS.find(':')+1, aTimeS.length()-1);
            string bTimeS = b.getScore();
            string newBTimeS = bTimeS.substr(0, bTimeS.find(':')) +
                bTimeS.substr(bTimeS.find(':')+1, bTimeS.length()-1);
            return stoi(newATimeS) < stoi(newBTimeS);
        });
    }
    //for debugging purposes
    void display() {
        for (int i = 0; i < players.size(); i++) {
            cout << players[i].getScore() << ", " << players[i].getName() << endl;
        }
    }
    bool isFastest(Player player) {
        string fastestTimeS = player.getScore();
        string newFastestTimeS = fastestTimeS;
        newFastestTimeS = fastestTimeS.substr(0, fastestTimeS.find(':')) +
            fastestTimeS.substr(fastestTimeS.find(':')+1, fastestTimeS.length()-1);
        int fastestTime = stoi(newFastestTimeS);
        for (int i = 0; i < players.size(); i++) {
            string playerTimeS = players[i].getScore();
            string newTimeS = playerTimeS.substr(0, playerTimeS.find(':')) +
                playerTimeS.substr(playerTimeS.find(':')+1, playerTimeS.length()-1);
            if (fastestTime < stoi(newTimeS)) {
                return true;
            }
        }
        return false;
    }


};

#endif //LEADERBOARD_H
