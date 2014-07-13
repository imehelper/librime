//
// Copyleft RIME Developers
// License: GPLv3
//
// 2011-05-15 GONG Chen <chen.sst@gmail.com>
//
#include <boost/foreach.hpp>
#include <rime/common.h>
#include <rime/config.h>
#include <rime/schema.h>
#include <rime/segmentation.h>
#include <rime/gear/abc_segmentor.h>

static const char kRimeAlphabet[] = "zyxwvutsrqponmlkjihgfedcba";

namespace rime {

AbcSegmentor::AbcSegmentor(const Ticket& ticket)
    : Segmentor(ticket), alphabet_(kRimeAlphabet) {
  // read schema settings
  if (!ticket.schema) return;
  if (Config *config = ticket.schema->config()) {
    config->GetString("speller/alphabet", &alphabet_);
    config->GetString("speller/delimiter", &delimiter_);
    config->GetString("speller/initials", &initials_);
    config->GetString("speller/finals", &finals_);
    ConfigListPtr extra_tags = config->GetList("abc_segmentor/extra_tags");
    if (extra_tags) {
      for (size_t i = 0; i < extra_tags->size(); ++i) {
        ConfigValuePtr value = As<ConfigValue>(extra_tags->GetAt(i));
        if (value)
          extra_tags_.insert(value->str());
      }
    }
  }
  if (initials_.empty()) {
    initials_ = alphabet_;
  }
}

bool AbcSegmentor::Proceed(Segmentation *segmentation) {
  const std::string &input(segmentation->input());
  DLOG(INFO) << "abc_segmentor: " << input;
  size_t j = segmentation->GetCurrentStartPosition();
  size_t k = j;
  bool expecting_an_initial = true;
  for (; k < input.length(); ++k) {
    bool is_letter = alphabet_.find(input[k]) != std::string::npos;
    bool is_delimiter =
        (k != j) && (delimiter_.find(input[k]) != std::string::npos);
    if (!is_letter && !is_delimiter)
      break;
    bool is_initial = initials_.find(input[k]) != std::string::npos;
    bool is_final = finals_.find(input[k]) != std::string::npos;
    if (expecting_an_initial && !is_initial && !is_delimiter) {
      break;  // not a valid seplling.
    }
    // for the next character.
    expecting_an_initial = is_final || is_delimiter;
  }
  DLOG(INFO) << "[" << j << ", " << k << ")";
  if (j < k) {
    Segment segment;
    segment.start = j;
    segment.end = k;
    segment.tags.insert("abc");
    BOOST_FOREACH(const std::string& tag, extra_tags_) {
      segment.tags.insert(tag);
    }
    segmentation->AddSegment(segment);
  }
  // continue this round
  return true;
}

}  // namespace rime
