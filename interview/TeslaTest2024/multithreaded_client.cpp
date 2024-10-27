#include "multithreaded_client.h"

std::optional<int> MultithreadedClient::getItemCount(std::string_view name) {
  std::string _name(name);
  std::shared_lock lck(mutex_items_);
  auto it = items_.find(_name);
  if (it == items_.end()) {
    return std::nullopt;
  }
  return it->second;
}

int MultithreadedClient::setItem(std::string_view name, int amount) {
  std::string _name(name);
  std::unique_lock lck(mutex_items_);
  auto it = items_.find(_name);
  if (it == items_.end()) {
    if (amount > 0) {
      items_[_name] = amount;
      return amount;
    } else {
      return 0;
    }
  } else {
    it->second += amount;
    if (it->second <= 0) {
      items_.erase(it);
      return 0;
    } else {
      return it->second;
    }
  }
  return 0;
}
