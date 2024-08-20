/*
The gaming company Sandstorm is developing an online two player game. You have been asked to implement the ranking system. All players have a rank determining their playing strength which gets updated after every game played. There are 25 regular ranks, and an extra rank, “Legend”, above that. The ranks are numbered in decreasing order, 25 being the
lowest rank, 1 the second highest rank, and Legend the highest rank.
Each rank has a certain number of “stars” that one needs to gain before advancing to the next rank. If a player wins a game, she gains a star. If before the game the player was on rank 6-25, and this was the third or more consecutive win, she gains an additional bonus star for that win. When she has all the stars for her rank (see list below) and gains another star, she will instead gain one rank and have one star on the new rank.
For instance, if before a winning game the player had all the stars on her current rank, she will after the game have gained one rank and have 1 or 2 stars (depending on whether she got a bonus star) on the new rank. If on the other hand she had all stars except one on a rank, and won a game that also gave her a bonus star, she would gain one rank and have 1 star on the new rank.
If a player on rank 1-20 loses a game, she loses a star. If a player has zero stars on a rank and loses a star, she will lose a rank and have all stars minus one on the rank below. However, one can never drop below rank 20 (losing a game at rank 20 with no stars will have no effect).
If a player reaches the Legend rank, she will stay legend no matter how many losses she incurs afterwards.
The number of stars on each rank are as follows: 
• Rank 25-21: 2 stars
• Rank 20-16: 3 stars
• Rank 15-11: 4 stars
• Rank 10-1: 5 stars

A player starts at rank 25 with no stars. Given the match history of a player, what is her rank at the end of the sequence of matches?
Input
The input consists of a single line describing the sequence of matches. Each character corresponds to one game; ‘W’represents a win and ‘L’ a loss. The length of the line is between 1 and 10 000 characters (inclusive).
Output
Output a single line containing a rank after having played the given sequence of games; either an integer between 1 and 25 or “Legend”.
*/

#include <bits/stdc++.h>

int starsNeeded(int rank) {
    //stars needed for each rank. see above
    if (rank > 20) return 2;
    if (rank > 15) return 3;
    if (rank > 10) return 4;
    return 5;
}

struct Player {
private:
    // Each player starts from rank 25
    int rank = 25;
    int stars = 0, consec_win = 0;
public:

    int getRank() {
        return this->rank;
    }

    std::string getRankStr() {
        // if the rank is 0, it should return "Legendary" instead
        std::stringstream ss;
        if (this->rank == 0) {
            ss << "Legendary";
        } else {
            ss << this->rank;
        }

        return ss.str();
    }

    int getStars() {
        return this->stars;
    }

    int getConsecWins() {
        return this->consec_win;
    }

    void addWin() {
        // nothing affects a legend
        if (rank == 0) return;

        // add a star and consecutive win
        ++stars;
        ++consec_win;

        // adds bonus stars gotten from consec wins
        this->addBonusStars();

        // keeps ranking up until not able to
        while (this->canRankUp()) {
            this->stars -= starsNeeded(this->rank);
            --this->rank;
        }
    }

    void addLoss() {
        // nothing affects a legend
        if (rank == 0) return;

        // lost the streak
        consec_win = 0;
        
        // rank protection for 20 - 25
        if (this->rank <= 20) {
            // losses a star if 1-20
            --this->stars;

            // rank down
            if (this->stars <= 0) {
                ++this->rank; this->stars = starsNeeded(this->rank) - 1;
            }
        }
    }

    void addBonusStars() {
        // no bonus for 0 - 6
        if (this->rank < 6) return;
        if (consec_win >= 3) ++stars;
    }

    bool canRankUp() {
        if (this->rank == 0) return false;
        return this->stars >= starsNeeded(this->rank);
    }
};

int main() {
    std::string matches;

    std::cout << "Write Match Data (W for win, L for loss): "; 
    std::cin >> matches;

    Player player;

    for (auto match : matches) {
        if (match == 'W') {
            player.addWin();
            std::cout << "Won! - ";
        } else {
            player.addLoss();
            std::cout << "Lost! - ";
        }

        std::cout << "Rank: " << player.getRankStr() << ", Stars: " <<  player.getStars() << ", Consec Wins" << player.getConsecWins() << std::endl;

        if (player.getRank() == 0) {
            break;
        }
    }

    std::cout << "Final Rank: " << player.getRankStr() << std::endl;
}