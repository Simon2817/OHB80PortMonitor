#pragma once
#include <QHash>
#include <QLinkedList>
#include <QPair>
#include <functional>

template<typename Key, typename Value>
class CommLRUCache
{
public:
    explicit CommLRUCache(int capacity) : m_capacity(capacity) {}

    bool contains(const Key &key) const { return m_map.contains(key); }
    int  size()     const { return m_map.size(); }
    int  capacity() const { return m_capacity; }

    bool get(const Key &key, Value &out)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return false;
        // 用移动语义将节点内容搬到头部，再回填原位置用于 erase；
        // 避免对大 Value 的深拷贝（Qt 隐式共享容器拷贝虽然是 O(1) 的浅拷贝，
        // 但 QLinkedList::prepend(pair) 仍会多产生一个临时 QPair 节点拷贝）。
        auto oldIt = it.value();
        m_list.prepend(qMakePair(oldIt->first, std::move(oldIt->second)));
        m_list.erase(oldIt);
        m_map[key] = m_list.begin();
        out = m_list.begin()->second;   // 对 Qt 隐式共享容器是 O(1) 浅拷贝
        return true;
    }

    // 命中则返回指向节点内 Value 的指针（完全零拷贝），未命中返回 nullptr。
    // 调用方在下一次该缓存的 put/remove/get 前不得再使用该指针。
    Value *getPtr(const Key &key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return nullptr;
        auto oldIt = it.value();
        m_list.prepend(qMakePair(oldIt->first, std::move(oldIt->second)));
        m_list.erase(oldIt);
        m_map[key] = m_list.begin();
        return &m_list.begin()->second;
    }

    bool put(const Key &key, const Value &value,
             Key *evictedKey = nullptr, Value *evictedValue = nullptr)
    {
        auto it = m_map.find(key);
        if (it != m_map.end()) {
            m_list.erase(it.value());
            m_map.remove(key);
        }
        bool evicted = false;
        if (m_map.size() >= m_capacity) {
            auto last = m_list.end(); --last;
            if (evictedKey)   *evictedKey   = last->first;
            if (evictedValue) *evictedValue = last->second;
            m_map.remove(last->first);
            m_list.erase(last);
            evicted = true;
        }
        m_list.prepend(qMakePair(key, value));
        m_map.insert(key, m_list.begin());
        return evicted;
    }

    // 一次性释放所有缓存条目（用于空闲超时回收内存）
    void clear()
    {
        m_map.clear();
        m_list.clear();
    }

    bool remove(const Key &key)
    {
        auto it = m_map.find(key);
        if (it == m_map.end()) return false;
        m_list.erase(it.value());
        m_map.erase(it);
        return true;
    }

    QList<Key> keys() const
    {
        QList<Key> result;
        for (auto &p : m_list) result.append(p.first);
        return result;
    }

private:
    int m_capacity;
    using ListType = QLinkedList<QPair<Key, Value>>;
    ListType m_list;
    QHash<Key, typename ListType::iterator> m_map;
};
