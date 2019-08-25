#pragma once

#include "sender.h"

struct withdraw {
  std::string account;
  unsigned amount;
  mutable messaging::sender atm_queue;

  withdraw(std::string const& account, unsigned amount,
           messaging::sender atm_queue)
      : account(account), amount(amount), atm_queue(atm_queue) {}
};

struct withdraw_ok {};

struct withdraw_denied {};

struct cancel_withdrawal {
  std::string account;
  unsigned amount;

  cancel_withdrawal(std::string const& account, unsigned amount)
      : account(account), amount(amount) {}
};

struct withdrawal_processed {
  std::string account;
  unsigned amount;

  withdrawal_processed(std::string const& account, unsigned amount)
      : account(account), amount(amount) {}
};

struct card_inserted {
  std::string account;

  explicit card_inserted(std::string const& account) : account(account) {}
};

struct digit_pressed {
  char digit;

  explicit digit_pressed(char digit) : digit(digit) {}
};

struct clear_last_pressed {};

struct eject_card {};

struct withdraw_pressed {
  unsigned amount;

  explicit withdraw_pressed(unsigned amount) : amount(amount) {}
};

struct cancel_pressed {};

struct issue_money {
  unsigned amount;
  issue_money(unsigned amount) : amount(amount) {}
};

struct verify_pin {
  std::string account;
  std::string pin;
  mutable messaging::sender atm_queue;

  verify_pin(std::string const& account, std::string const& pin,
             messaging::sender atm_queue)
      : account(account), pin(pin), atm_queue(atm_queue) {}
};

struct pin_verified {};

struct pin_incorrect {};

struct display_enter_pin {};

struct display_enter_card {};

struct display_insufficient_funds {};

struct display_withdrawal_cancelled {};

struct display_pin_incorrect_message {};

struct display_withdrawal_options {};

struct get_balance {
  std::string account;
  mutable messaging::sender atm_queue;

  get_balance(std::string const& account, messaging::sender atm_queue)
      : account(account), atm_queue(atm_queue) {}
};

struct balance {
  unsigned amount;

  explicit balance(unsigned amount) : amount(amount) {}
};

struct display_balance {
  unsigned amount;

  explicit display_balance(unsigned amount) : amount(amount) {}
};

struct balance_pressed {};
