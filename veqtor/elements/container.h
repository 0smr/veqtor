#pragma once

#include <QObject>
#include <QPointer>
#include <QQmlListProperty>

#include "../shapes/shapes.h"
#include "element.h"

namespace veqtor::elements {
using el_ptr = QPointer<element>;

class container: public element {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<element> children READ childrenList NOTIFY childrenListChanged)
public:
    explicit container(QObject *parent = nullptr,
                       const QMap<QString, QString> &attrs = {},
                       const std::vector<el_ptr> &els = {})
        : element{parent, attrs}, mChildren(els.begin(), els.end()) {}

    /// operators
    const el_ptr &operator[](size_t i) const { return mChildren[i]; }
    el_ptr &operator[](size_t i) { return mChildren[i]; }

    /// setters
    virtual void push_back(const el_ptr &childElement) {
        mChildren.push_back(childElement);
    }

    /// getters
    QVector<el_ptr>::iterator begin() { return mChildren.begin(); }
    QVector<el_ptr>::iterator end() { return mChildren.end(); }
    QVector<el_ptr>::const_iterator cbegin() { return mChildren.cbegin(); }
    QVector<el_ptr>::const_iterator cend() { return mChildren.cend(); }

    bool empty() { return mChildren.empty(); }
    int size() { return mChildren.size(); }

    const QQmlListProperty<element> childrenList() {
        using qq_list_prop = QQmlListProperty<element>;
        static auto cast = [](qq_list_prop *l){ return reinterpret_cast<QVector<el_ptr>*>(l->data); };
        static auto repl = [](qq_list_prop *l, int i, element *el){ cast(l)->replace(i, el); };
        static auto at   = [](qq_list_prop *l, int i){ return cast(l)->at(i).data(); };
        static auto app  = [](qq_list_prop *l, element *el){ cast(l)->push_back(el); };
        static auto pop  = [](qq_list_prop *l){ return cast(l)->pop_back(); };
        static auto clr  = [](qq_list_prop *l){ return cast(l)->clear(); };
        static auto size = [](qq_list_prop *l){ return cast(l)->size(); };
        return {this, &mChildren, app, size, at, clr, repl, pop};
    }

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
    void childrenListChanged();
protected:
    QVector<el_ptr> mChildren;
};
}
