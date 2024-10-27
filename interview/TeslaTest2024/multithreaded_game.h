#pragma once

#include "Game.h"
#include "multithreaded_client.h"
#include <unordered_set>
#include <shared_mutex>

class MultithreadedGame : public Game {
public:
  /**
   * Creates new client instance as player entered the game.
   *
   * After calling it should return a non-null pointer to internal collection of
   * clients. Increases client counter.
   */
  Client *onJoin() override;
  /**
   * Players receives item from the ground. If items `name` matches any item
   * already in player's bag, then increase it's count, otherwise add new item.
   *
   * Does nothing if user is not connected, or given item does not exists.
   */
  void onGetItem(Client *user, std::string_view name, int amount) override;
  /**
   * Player wants to drop item on the ground - remove it from client's list of
   * items.
   * Should return remaining item count or `std::nullopt` if operation did not
   * work.
   */
  void onDropItem(Client *fromClientPtr, std::string_view name,
                  int amount) override;
  /**
   * Player `fromClientPtrPtr` is transferring `amount` of item `name` to
   * `toClientPtr`.
   */
  void onGiveItem(Client *fromClientPtr, Client *toClientPtr,
                  std::string_view name, int amount) override;
  /**
   * Player is leaving the game.
   * Does nothing if `clientPtr` is not connected, otherwise this client is
   * removed and amount of connected clients is decreased.
   */
  void onLeave(Client *clientPtr) override;

  /**
   * Returns how many clients are connected.
   */
  std::size_t getClientCount() const override;

private:
  std::unordered_set<Client*>       clients_;
  mutable std::shared_mutex         mutex_clients_;
};
