#include <iostream>

#include "multithreaded_client.h"
#include "multithreaded_game.h"

Client *MultithreadedGame::onJoin() {
  // std::cout << "This is debug output" << std::endl;
  Client* client = new MultithreadedClient;
  std::unique_lock lck(mutex_clients_);
  clients_.insert(client);
  return client;
}

void MultithreadedGame::onGetItem(Client *clientPtr, std::string_view name,
                                  int amount) {
  std::shared_lock lck(mutex_clients_);
  auto it = clients_.find(clientPtr);
  if (it == clients_.end()) { return; }
  MultithreadedClient* client = dynamic_cast<MultithreadedClient*>(clientPtr);
  if (client) {
    client->setItem(name, amount);
  }
}

void MultithreadedGame::onDropItem(Client *clientPtr, std::string_view name,
                                   int amount) {
  amount *= -1;
  std::shared_lock lck(mutex_clients_);
  auto it = clients_.find(clientPtr);
  if (it == clients_.end()) { return; }
  MultithreadedClient* client = dynamic_cast<MultithreadedClient*>(clientPtr);
  if (client) {
    client->setItem(name, amount);
  }
}

void MultithreadedGame::onGiveItem(Client *fromClientPtr, Client *toClientPtr,
                                   std::string_view name, int amount) {
  std::shared_lock lck(mutex_clients_);
  if (clients_.count(fromClientPtr) == 0 || clients_.count(toClientPtr) == 0)
  { return; }
  MultithreadedClient* src = dynamic_cast<MultithreadedClient*>(fromClientPtr);
  MultithreadedClient* dst = dynamic_cast<MultithreadedClient*>(toClientPtr);
  if (!src || !dst) { return; }
  int count = src->setItem(name, -1 * amount);
  dst->setItem(name, amount);
  // if (count > 0) {
    // dst->setItem(name, count);
  // }
}

void MultithreadedGame::onLeave(Client *clientPtr) {
  std::unique_lock lck(mutex_clients_);
  if (clients_.erase(clientPtr) != 0) {
      delete clientPtr;
  }
}

std::size_t MultithreadedGame::getClientCount() const { 
  std::shared_lock lck(mutex_clients_);
  return clients_.size(); 
}
