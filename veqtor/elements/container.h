#pragma once

#include <QObject>
#include <QPointer>

#include "../shapes/shapes.h"
#include "element.h"

namespace veqtor::elements {
using el_ptr = QPointer<element>;

class container: public element {
    Q_OBJECT
public:
    explicit container(QObject *parent = nullptr,
                       const QMap<QString, QString> &attrs = {},
                       const std::vector<el_ptr> &els = {})
        : element{parent, attrs}, mElements(els) {}

    /// operators
    const el_ptr &operator[](size_t i) const { return mElements[i]; }
    el_ptr &operator[](size_t i) { return mElements[i]; }

    virtual void push_back(const el_ptr &childElement) {
        mElements.push_back(childElement);
    }

    /// getters
    std::vector<el_ptr>::iterator begin() { return mElements.begin(); }
    std::vector<el_ptr>::iterator end() { return mElements.end(); }
    std::vector<el_ptr>::const_iterator cbegin() { return mElements.cbegin(); }
    std::vector<el_ptr>::const_iterator cend() { return mElements.cend(); }

    bool empty() { return mElements.empty(); }
    bool size() { return mElements.size(); }

    virtual Type type() const override { return element::Container; }
    virtual bool contains(const QPointF& point) const override {
        Q_UNUSED(point)
        return false;
    }

    void walk(const std::function<void(const el_ptr &)> &func) {
        walk(el_ptr(this), func);
    }

    /**
     * @brief Any
     * @abstract Pre-order traversal of the tree till the first `true` return value.
     * @param func, Callback function.
     * @see traversal.
     */
    bool any(const std::function<bool(const el_ptr &)> &cb) {
        return traversal<false>(el_ptr{this}, cb);
    }

    /**
     * @brief All
     * @abstract Pre-order traversal of the tree till the first `false` return value.
     * @param func, Callback function.
     * @see traversal.
     */
    bool all(const std::function<bool(const el_ptr &)> &cb) {
        return traversal<true>(el_ptr{this}, cb);
    }

    /**
     * @brief Tree traversal
     * @abstract Traversal of the tree in pre-order.
     * @param root, The tree root element.
     * @param func, Callback function.
     */
    template <bool _all = true>
    static bool traversal(const el_ptr &root, const std::function<bool(const el_ptr &)> &func) {
        if(_all ^ func(root)) return !_all;
        if(root->type() > element::Container) {
            auto cont = dynamic_cast<container*>(root.data());
            for(const auto &child: *cont) if(_all ^ traversal(child, func)) return !_all;
        }
        return _all;
    }

    static void walk(const el_ptr &root, const std::function<void(const el_ptr &)> &func) {
        func(root);
        if(root->type() > element::Container) {
            auto cont = dynamic_cast<container*>(root.data());
            for(const auto &child: *cont) walk(child, func);
        }
    }
signals:
protected:
    std::vector<QPointer<element>> mElements;
};
}
