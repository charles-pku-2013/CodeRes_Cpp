#pragma once

#include "Client.h"
#include <unordered_map>
#include <shared_mutex>

class MultithreadedClient : public Client {
public:
  /**
   * Find an item in the bag, and return its count.
   * Returns if the item is missing.
   */
  std::optional<int> getItemCount(std::string_view name) override;

  int setItem(std::string_view name, int amount);

private:
  std::unordered_map<std::string, int>      items_;
  mutable std::shared_mutex                 mutex_items_;
};
