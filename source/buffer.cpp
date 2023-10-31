#include "../include/buffer.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#define MAX_SIZE 10

Buffer::Buffer() {}

Buffer::~Buffer() {
  for (size_t i = 0; i < this->size(); ++i) {
    delete this->data[i];
  }
}

void Buffer::clear() {
  this->~Buffer();
  this->data.clear();
}

std::wstring &Buffer::operator[](const size_t &index) const {
  if (index >= this->size()) {
    throw std::out_of_range("");
  } else {
    return *(this->data[index]);
  }
}

size_t Buffer::size() const { return this->data.size(); }

std::wstring Buffer::get() const {
  std::wstring data;

  for (size_t i = 0; i < this->size(); ++i) {
    data += (*this)[i] + (wchar_t)'\n';
  }
  if (!data.empty()) {
    data.pop_back();
  }

  return data;
}

void Buffer::insert(const size_t &index, const std::wstring &to_insert) {
  if (index > this->size()) {
    throw std::out_of_range("");
  } else {
    if (to_insert.size() > MAX_SIZE) {
      std::vector<std::wstring> v;
      for (size_t i = 0; i < to_insert.size(); i += MAX_SIZE) {
        v.push_back(
            to_insert.substr(i, std::min(i + MAX_SIZE, to_insert.size())));
      }
      this->insert(index, v);
    } else {
      if (index == this->size()) {
        this->data.push_back(nullptr);
      } else {
        this->data.insert(data.begin() + index, nullptr);
      }
      this->override(index, to_insert);
    }
  }
}
void Buffer::insert(const size_t &index, const size_t &pos,
                    const std::wstring &to_insert) {
  if (index >= this->size() || pos > (*this)[index].size()) {
    throw std::out_of_range("");
  } else {
    std::wstring s((*this)[index].size() + to_insert.size(), '\0');

    for (size_t i = 0; i < pos; ++i) {
      s[i] = (*this)[index][i];
    }
    for (size_t i = 0; i < to_insert.size(); ++i) {
      s[i + pos] = to_insert[i];
    }
    for (size_t i = 0; i < s.size() - pos - to_insert.size(); ++i) {
      s[i + pos + to_insert.size()] = (*this)[index][i + pos];
    }

    override(index, s);
  }
}

void Buffer::insert(const size_t &index,
                    const std::vector<std::wstring> &to_insert) {
  for (int i = to_insert.size() - 1; i >= 0; --i) {
    this->insert(0, to_insert[i]);
  }
}

void Buffer::erase(const size_t &index) {
  if (index >= this->size()) {
    throw std::out_of_range("");
  } else {
    delete this->data[index];
    this->data.erase(this->data.begin() + index);
  }
}

void Buffer::erase(size_t l, size_t r) {
  if (l > r) {
    std::swap(l, r);
  }

  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      delete this->data[i];
    }

    this->data.erase(this->data.begin() + l, this->data.begin() + r + 1);
  }
}

void Buffer::replace(const size_t &index, const size_t &pos,
                     const wchar_t &to_write) {
  if (pos >= this->size()) {
    throw std::out_of_range("");
  } else {
    (*this)[index][pos] = to_write;
  }
}

void Buffer::replace(size_t l, size_t r, const std::wstring &to_replace,
                     const std::wstring &to_write) {
  if (l > r) {
    std::swap(l, r);
  }

  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      std::wstring s((*this)[i]);
      str_replace(s, to_replace, to_replace);
      this->override(i, s);
    }
  }
}

void Buffer::eraseLeadingZeros(size_t l, size_t r) {
  if (l > r) {
    std::swap(l, r);
  }

  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      std::wstring s((*this)[i]);
      erase_leading_zeros(s);
      this->override(i, s);
    }
  }
}

void Buffer::eraseNonDecreasing(size_t l, size_t r) {
  if (l > r) {
    std::swap(l, r);
  }

  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      std::wstring s((*this)[i]);
      erase_non_decreasing(s);
      this->override(i, s);
    }
  }
}

void Buffer::reduceRepetitiveAsterisk(size_t l, size_t r) {
  if (l > r) {
    std::swap(l, r);
  }

  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      std::wstring s((*this)[i]);
      reduce_repetitive_asterisk(s);
      this->override(i, s);
    }
  }
}

void Buffer::eraseCurlyBrackets(size_t l, size_t r) {
  if (l > r) {
    std::swap(l, r);
  }
  if (r >= this->size()) {
    throw std::out_of_range("");
  } else {
    for (size_t i = l; i <= r; ++i) {
      std::wstring s((*this)[i]);
      erase_curly_brackets(s);
      this->override(i, s);
    }
  }
}

void Buffer::override(const size_t &index, const std::wstring &to_write) {
  delete this->data[index];
  this->data[index] = new std::wstring(to_write);
}

inline void str_replace(std::wstring &s, const std::wstring &to_replace,
                        const std::wstring &to_write) {
  size_t pos = 0;

  while ((pos = s.find(to_replace, pos)) != -1) {
    s.erase(pos, to_replace.size());
    s.insert(pos, to_write);
    pos += to_write.size();
  }
}
inline void erase_leading_zeros(std::wstring &s) {
  bool prev_zero = 0, to_erase = 1;

  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '0') {
      if (to_erase && prev_zero) {
        s.erase(--i, 1);
      }

      prev_zero = 1;
    } else if (s[i] >= '1' && s[i] <= '9') {
      if (to_erase && prev_zero) {
        s.erase(--i, 1);
      }

      to_erase = 0;
    } else {
      prev_zero = 0;
      to_erase = 1;
    }
  }
}
inline void erase_non_decreasing(std::wstring &s) {
  bool to_erase = 0;
  int prev = -1;
  size_t l = -1;

  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] >= '0' && s[i] <= '9') {
      if (l == -1) {
        l = i;
      }
      int cur = s[i] - '0';
      if (cur < prev) {
        to_erase = 1;
      }
      prev = cur;
    } else {
      if (l != -1) {
        if (to_erase) {
          s.erase(l, i - l);
        }
        l = -1;
      }
    }
  }
  if (l != -1 && to_erase) {
    s.erase(l, s.size() - l);
  }
}
inline void reduce_repetitive_asterisk(std::wstring &s) {
  size_t l = -1, r;

  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '*') {
      if (l == -1) {
        l = i;
      }

      r = i;
    } else {
      if (l != -1) {
        s.erase(l, (r - l + 1) / 2);

        i -= (r - l + 1) / 2 - 1;
        l = -1;
      }
    }
  }

  if (l != -1) {
    s.erase(l, (r - l + 1) / 2);
  }
}
inline void erase_curly_brackets(std::wstring &s) {
  size_t l = -1;

  for (size_t i = 0; i < s.size(); ++i) {
    if (s[i] == '{') {
      l = i;
    } else if (s[i] == '}' && l != -1) {
      s.erase(l, i - l + 1);
      l = -1;
    }
  }
}
