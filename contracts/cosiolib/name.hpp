#pragma once

#include <cosiolib/types.hpp>

#define COSIO_CONTRACT_NAME_PREFIX_CHAR  '$'
#define COSIO_CONTRACT_NAME_SPLIT_CHAR   '@'

namespace cosio {

class name {
public:
    name() { }

    name(const name& other) {
        set_name(other);
    }

    name(const std::string& s) {
        set_string(s);
    }

    void set_name(const name& other) {
        _name = other._name;
        _split = other._split;
    }

    void set_account(const account_name& account) {
        _name = account;
        _split = std::string::npos;
    }

    void set_contract(const account_name& owner, const contract_name& contract) {
        _name = COSIO_CONTRACT_NAME_PREFIX_CHAR + contract + COSIO_CONTRACT_NAME_SPLIT_CHAR + owner;
        _split = 1 + contract.size();
    }

    void set_string(const std::string& s) {
        if (s.size() > 1 && s[0] == COSIO_CONTRACT_NAME_PREFIX_CHAR) {
            auto split = s.find(COSIO_CONTRACT_NAME_SPLIT_CHAR);
            if ( split != std::string::npos ) {
                _name = s;
                _split = split;
                return;
            }
        }
        _name = s;
        _split = std::string::npos;
    }

    bool is_contract() const {
        return _split != std::string::npos
                && _name.size() > 1 + _split
                && _name[0] == COSIO_CONTRACT_NAME_PREFIX_CHAR
                && _name[_split] == COSIO_CONTRACT_NAME_SPLIT_CHAR;
    }

    account_name account() const {
        return is_contract()? _name.substr(_split + 1) : _name;
    }

    contract_name contract() const {
        return is_contract()? _name.substr(1, _split - 1) : "";
    }

    const std::string& string() const {
        return _name;
    }
    
    name& operator = (const name& other) {
        set_name(other);
        return *this;
    }

    name& operator = (const std::string& s) {
        set_string(s);
        return *this;
    }

    template<typename DataStream>
    friend DataStream& operator << (DataStream& ds, const name& n) {
        return ds << n.string();
    }

    template<typename DataStream>
    friend DataStream& operator >> (DataStream& ds, name& n) {
        std::string s;
        ds >> s;
        n.set_string(s);
        return ds;
    }

private:
    std::string _name;
    std::size_t _split = std::string::npos;
};

}
