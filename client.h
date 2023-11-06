#ifndef CLIENT_H
#define CLIENT_H

struct Client {
    int id;
    int demand;
    bool isOnSolution;

    Client(int _id, int _demand, bool _isOnSolution) : id(_id), demand(_demand), isOnSolution(_isOnSolution) {}
};

#endif