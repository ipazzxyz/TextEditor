#pragma once
#include <string>
#include <vector>

class Buffer {
public:
  Buffer();
  ~Buffer();
  std::wstring &operator[](const size_t &) const;
  size_t size() const;
  std::wstring get() const;
  void clear();

  void insert(const size_t &, const std::wstring &);
  void insert(const size_t &, const size_t &, const std::wstring &);
  void insert(const size_t &, const std::vector<std::wstring> &);

  void erase(const size_t &);
  void erase(size_t, size_t);

  void replace(const size_t &, const size_t &, const wchar_t &);
  void replace(size_t, size_t, const std::wstring &, const std::wstring &);

  void eraseLeadingZeros(size_t, size_t);
  void eraseNonDecreasing(size_t, size_t);
  void reduceRepetitiveAsterisk(size_t, size_t);
  void eraseCurlyBrackets(size_t, size_t);

private:
  std::vector<std::wstring *> data;

  void override(const size_t &, const std::wstring &);
};

inline void str_replace(std::wstring &, const std::wstring &,
                        const std::wstring &);
inline void erase_leading_zeros(std::wstring &);
inline void erase_non_decreasing(std::wstring &);
inline void reduce_repetitive_asterisk(std::wstring &);
inline void erase_curly_brackets(std::wstring &);
