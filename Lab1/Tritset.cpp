
#include "Tritset.h"
#include <conio.h>

namespace trit_set {

	TritSet::TritSet() : start(nullptr), size(0), new_pos(0), last_chahged_trit(0) {}

	TritSet::TritSet(size_t set_size) : size(set_size), new_pos(0), last_chahged_trit(0) {
		uint real_size = (uint) ceil((double)(set_size + 1) * 2.0 / 8.0 / sizeof(uint));
		std::cout << real_size << std::endl;
		start = new uint[real_size];
		for (size_t i = 0; i < real_size; i++) {
			start[i] = 0;
		}
	}

	TritSet::TritSet(const TritSet &tset) : size(tset.size), new_pos(tset.new_pos),
		last_chahged_trit(tset.last_chahged_trit) {
		uint real_size = (uint) ceil((double)(tset.size + 1) * 2.0 / 8.0 / sizeof(uint)) + 1;
		start = new uint[real_size];
		for (size_t i = 0; i < real_size; i++) {
			start[i] = tset.start[i];
		}
	}

	TritSet::~TritSet() {
		delete[] start;
		start = nullptr;
	}

	TritSet& TritSet::operator=(const TritSet& tset) {
		if (this != &tset) {
			delete[] start;
			uint real_size = (uint) ceil((double)(tset.size + 1) * 2.0 / 8.0 / sizeof(uint));
			start = new uint[real_size];
			for (size_t i = 0; i < real_size; i++) {
				start[i] = tset.start[i];
			}
			size = tset.size;
			new_pos = tset.new_pos;
			last_chahged_trit = tset.last_chahged_trit;
		}
		return *this;
	}

	Trit TritSet::operator[](size_t position) const {
		if (position < 0 || position > size) {
			throw my_exception("Error: trit index is outside tritset");
			_getch();
		}
		uint *trit_ptr = start + position * 2 / 8 / sizeof(uint);
		int trit_index = position - (position * 2 / 8 / sizeof(uint)) * sizeof(uint) * 8 / 2;
		int t = (*trit_ptr >> trit_index * 2) & (uint)3;
		if (t == 0) { return Unknown; }
		if (t == 1) { return True; }
		else { return False; }
	}

	TritSet::reference TritSet::operator[](size_t position) {
		if (position <= size) {
			this->last_chahged_trit = position;
			uint *trit_ptr = start + position * 2 / 8 / sizeof(uint);
			int trit_index = position - (position * 2 / 8 / sizeof(uint)) * sizeof(uint) * 8 / 2;
			TritSet::reference ref(trit_ptr, trit_index, this);
			return ref;
		}
		else {
			new_pos = position;
			TritSet::reference ref(NULL, -1, this);
			return ref;
		}
	}

	TritSet::reference::reference(uint *trit_ptr, int trit_index, TritSet *trit_set) :
		ptr(trit_ptr), index(trit_index), tset(trit_set) {}
	
	constexpr size_t trit_position(size_t position) {
		return position % (sizeof(uint) * 4);
	}
	constexpr size_t word_position(size_t position) {
		return position / (sizeof(uint) * 4);
	}
	TritSet::reference::operator Trit() const {
		if (index >= (int)tset->size) return Unknown;
		return static_cast<Trit>(static_cast<uint>(3 & (* (tset->start + word_position(index)) >> trit_position(index) * 2)));	
	}

	TritSet::reference& TritSet::reference::operator=(Trit t) {
		if (this->tset->new_pos > this->tset->size && t == Unknown) {
			return *this;
		}
		if (this->tset->new_pos > this->tset->size) {
			uint new_real_size = (uint)ceil((double)(this->tset->new_pos + 1) * 2.0 / 8.0 / sizeof(uint));
			uint *tmp = new uint[new_real_size];
			for (size_t i = 0; i < new_real_size; i++) {
				tmp[i] = 0;
			}
			for (size_t i = 0; i <= this->tset->new_pos; i++) {
				size_t ptr_number = i * 2 / sizeof(uint) / 8;
				size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
				if (i < this->tset->size) {
					uint t = (this->tset->start[ptr_number] >> 2 * trit_pos) & (uint)3;
					tmp[ptr_number] = tmp[ptr_number] | (t << 2 * trit_pos);
				}
			}
			delete[] this->tset->start;
			this->tset->start = tmp;
			this->tset->size = this->tset->new_pos;
			this->tset->last_chahged_trit = this->tset->new_pos;
			this->ptr = this->tset->start + (this->tset->size * 2 / 8 / sizeof(uint));
			this->index = this->tset->size - (this->tset->size * 2 / 8 / sizeof(uint)) * sizeof(uint) * 8 / 2;
		}
		*ptr = *ptr & ~((uint)3 << 2 * index);
		*ptr = *ptr | ((uint)t << 2 * index);

		return *this;
	}

	TritSet::reference& TritSet::reference::operator=(const reference &other) {
		*this = static_cast<Trit>(other);
		return *this;
	}

	bool TritSet::reference::operator==(const TritSet::reference& one)  const {
		return (ptr == one.ptr && index == one.index);
	}

	bool TritSet::reference::operator==(const Trit& one) const {
		if (this->index != -1) {
			uint t = (*(this->ptr) >> 2 * this->index) & ((uint)3);
			return (t == one);
		}
		else return false;
	}
	
	void TritSet::shrink() {
		uint real_size = (uint)ceil((double)(this->last_chahged_trit + 1) * 2.0 / 8.0 / sizeof(uint));
		uint *new_start = new uint[real_size];
		for (size_t i = 0; i < real_size; i++) {
			new_start[0] = 0;
		}
		for (size_t i = 0; i < last_chahged_trit; i++) {
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (start[ptr_number] >> 2 * trit_pos) & (uint)3;
			new_start[ptr_number] = new_start[ptr_number] | (t << 2 * trit_pos);
		}
		delete[] start;
		start = new_start;
		size = last_chahged_trit;
	}

	void TritSet::trim(size_t last_index) {
		uint real_size = (uint)ceil((double)(last_index + 1) * 2.0 / 8.0 / sizeof(uint));
		uint *new_start = new uint[real_size];
		for (size_t i = 0; i < real_size; i++) {
			new_start[0] = 0;
		}
		for (size_t i = 0; i <= last_index; i++) {
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (start[ptr_number] >> 2 * trit_pos) & (uint)3;
			new_start[ptr_number] = new_start[ptr_number] | (t << 2 * trit_pos);
		}
		delete[] start;
		start = new_start;
		size = last_index;
	}

	size_t TritSet::length() {
		for (size_t i = this->size - 1; i >= 0; i--) {
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (start[ptr_number] >> 2 * trit_pos) & (uint)3;
			if (t != 0) {
				return i + 1;
			}
		}
		return 0;
	}

	size_t TritSet::capacity() {
		return this->size;
	}

	size_t TritSet::cardinality(Trit value) {
		size_t trit_counter = 0;
		for (size_t i = 0; i < this->size; i++) {
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (start[ptr_number] >> 2 * trit_pos) & (uint)3;
			if (t == value) {
				trit_counter++;
			}
		}
		return trit_counter;
	}

	std::unordered_map< Trit, size_t, std::hash<int> > TritSet::cardinality() {
		std::unordered_map< Trit, size_t, std::hash<int> > m;
		size_t trits_true = this->cardinality(True);
		size_t trits_false = this->cardinality(False);
		size_t trits_unknown = this->cardinality(Unknown);
		m[True] = trits_true;
		m[False] = trits_false;
		m[Unknown] = trits_unknown;
		return m;
	};

	TritSet operator&(const TritSet A, const TritSet B) {
		if (A.size >= B.size) {
			TritSet C(A.size);
			for (size_t i = 0; i < A.size; i++) {
				size_t ptr_number = i * 2 / sizeof(uint) / 8;
				size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
				if (i < B.size) {
					uint t_A = (A.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					uint t_B = (B.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					if (t_A == 2 || t_B == 2) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)2 << 2 * trit_pos);
					}
					else if (t_A == t_B && t_A == 1) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)1 << 2 * trit_pos);
					}
				}
				else {
					uint t = (A.start[ptr_number] >> 2 * trit_pos) & (uint)3;
					C.start[ptr_number] = C.start[ptr_number] | (t << 2 * trit_pos);
				}
			}
			C.last_chahged_trit = B.size;
			return C;
		}
		else {
			TritSet C(B.size);
			for (size_t i = 0; i < A.size; i++) {
				size_t ptr_number = i * 2 / sizeof(uint) / 8;
				size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
				if (i < A.size) {
					uint t_A = (A.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					uint t_B = (B.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					if (t_A == 2 || t_B == 2) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)2 << 2 * trit_pos);
					}
					else if (t_A == t_B && t_A == 1) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)1 << 2 * trit_pos);
					}
				}
				else {
					uint t = (A.start[ptr_number] >> 2 * trit_pos) & (uint)3;
					C.start[ptr_number] = C.start[ptr_number] | (t << 2 * trit_pos);
				}
			}
			C.last_chahged_trit = B.size;
			return C;
		}
	}
	TritSet operator|(const TritSet A, const TritSet B) {
		if (A.size >= B.size) {
			TritSet C(A.size);
			for (size_t i = 0; i < A.size; i++) {
				size_t ptr_number = i * 2 / sizeof(uint) / 8;
				size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
				if (i < B.size) {
					uint t_A = (A.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					uint t_B = (B.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					if (t_A == 1 || t_B == 1) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)1 << 2 * trit_pos);
					}
					else if (t_A == t_B && t_A == 2) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)2 << 2 * trit_pos);
					}
				}
				else {
					uint t = (A.start[ptr_number] >> 2 * trit_pos) & (uint)3;
					C.start[ptr_number] = C.start[ptr_number] | (t << 2 * trit_pos);
				}
			}
			C.last_chahged_trit = B.size;
			return C;
		}
		else {
			TritSet C(B.size);
			for (size_t i = 0; i < A.size; i++) {
				size_t ptr_number = i * 2 / sizeof(uint) / 8;
				size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
				if (i < A.size) {
					uint t_A = (A.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					uint t_B = (B.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
					if (t_A == 1 || t_B == 1) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)1 << 2 * trit_pos);
					}
					else if (t_A == t_B && t_A == 2) {
						C.start[ptr_number] = C.start[ptr_number] | ((uint)2 << 2 * trit_pos);
					}
				}
				else {
					uint t = (A.start[ptr_number] >> 2 * trit_pos) & (uint)3;
					C.start[ptr_number] = C.start[ptr_number] | (t << 2 * trit_pos);
				}
			}
			C.last_chahged_trit = B.size;
			return C;
		}
	}

	TritSet operator~(const TritSet A) {
		TritSet C(A.size);
		for (size_t i = 0; i < A.size; i++) {
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (~A.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
			if (t != 3) { // 3 - ??????????????? 0 (Unknown)
				C.start[ptr_number] = C.start[ptr_number] & ~((uint)3 << 2 * trit_pos);
				C.start[ptr_number] = C.start[ptr_number] | (t << 2 * trit_pos);
			}
		}
		C.last_chahged_trit = C.size;
		return C;
	}

	std::ostream& operator<<(std::ostream& os, const TritSet& tset) {
		std::string trit;
		for (size_t i = 0; i <= tset.size; i++)
		{
			size_t ptr_number = i * 2 / sizeof(uint) / 8;
			size_t trit_pos = i - ptr_number * sizeof(uint) * 8 / 2;
			uint t = (tset.start[ptr_number] >> 2 * trit_pos) & ((uint)3);
			if (t == 0) {
				trit = "Unknown";
			}
			else if (t == 1) {
				trit = "True";
			}
			else {
				trit = "False";
			}
			if (i != tset.size) {
				os << trit << ", ";
			}
			else {
				os << trit;
			}
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const TritSet::reference& ref) {
		std::string trit;
		uint t = (*ref.ptr >> 2 * ref.index) & ((uint)3);
		if (t == 0) {
			trit = "Unknown";
		}
		else if (t == 1) {
			trit = "True";
		}
		else {
			trit = "False";
		}
		os << trit;
		return os;
	}

	my_exception::my_exception(const std::string& msg) : message(msg) {}
}