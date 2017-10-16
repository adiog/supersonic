// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsgen/supersonicBean/SupersonicBeans.h>

namespace supersonic {

template <typename Bean>
struct DecodeCursorAsIterator
{
    DecodeCursorAsIterator(Cursor *cursor)
            : cursor_(cursor)
    {
    }

    Cursor *cursor_;
    absl::optional<supersonic::ResultView> resultView_;

    void fetchNext()
    {
        resultView_ = cursor_->Next(1);
    }

    Bean operator*()
    {
        auto &view = resultView_->view();
        return SupersonicBeanTable<Bean>::decode(view);
    }

    void operator++()
    {
        fetchNext();
    }

    bool operator!=(const DecodeCursorAsIterator &doNotCompareToIt)
    {
        return this->hasMoreData();
    }

    bool hasMoreData()
    {
        return resultView_->has_data();
    }
};

template <typename Bean>
struct DecodeCursorAs
{
    DecodeCursorAs(Cursor *cursor)
            : cursor_(cursor)
    {
    }

    DecodeCursorAsIterator<Bean> begin()
    {
        DecodeCursorAsIterator<Bean> iterator(cursor_);
        iterator.fetchNext();
        return iterator;
    }

    DecodeCursorAsIterator<Bean> end()
    {
        return DecodeCursorAsIterator<Bean>(cursor_);
    }

    Cursor *cursor_;
};
}