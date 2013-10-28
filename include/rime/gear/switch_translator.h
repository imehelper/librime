//
// Copyleft RIME Developers
// License: GPLv3
//
// 2013-05-26 GONG Chen <chen.sst@gmail.com>
//
#ifndef RIME_SWITCH_TRANSLATOR_H_
#define RIME_SWITCH_TRANSLATOR_H_

#include <rime/translator.h>

namespace rime {

class SwitchTranslator : public Translator {
 public:
  SwitchTranslator(const Ticket& ticket);

  virtual shared_ptr<Translation> Query(const std::string &input,
                                        const Segment &segment,
                                        std::string* prompt);
};

}  // namespace rime

#endif  // RIME_SWITCH_TRANSLATOR_H_
