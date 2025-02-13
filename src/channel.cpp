#include "../includes/channel.hpp"
#include <algorithm>
#include <sys/socket.h> // Pour la fonction send()

// Constructeur : initialise le nom et définit le topic à vide
Channel::Channel(const std::string &name) : _name(name), _topic("") {}

// Destructeur (aucune libération spécifique à effectuer ici)
Channel::~Channel() {}

// Retourne le nom du canal
const std::string &Channel::getName() const {
    return _name;
}

// Retourne le topic du canal
const std::string &Channel::getTopic() const {
    return _topic;
}

// Retourne la liste des membres du canal
const std::vector<int> &Channel::getMembers() const {
    return _members;
}

// Modifie le topic du canal
void Channel::setTopic(const std::string &topic) {
    _topic = topic;
}

// Ajoute un membre au canal s'il n'est pas déjà présent
void Channel::addMember(int client_fd) {
    if (!isMember(client_fd))
        _members.push_back(client_fd);
}

// Retire un membre du canal
void Channel::removeMember(int client_fd) {
    _members.erase(std::remove(_members.begin(), _members.end(), client_fd), _members.end());
}

// Vérifie si un client est membre du canal
bool Channel::isMember(int client_fd) const {
    return std::find(_members.begin(), _members.end(), client_fd) != _members.end();
}

// Diffuse un message à tous les membres du canal sauf l'expéditeur
void Channel::broadcastMessage(const std::string &msg, int sender_fd) const {
    for (size_t i = 0; i < _members.size(); i++) {
        if (_members[i] != sender_fd) {
            send(_members[i], msg.c_str(), msg.size(), MSG_NOSIGNAL);
        }
    }
}
