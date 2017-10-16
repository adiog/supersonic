// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/types/optional.h>

#include "supersonic/cursor/core/sort.h"
#include "supersonic/cursor/infrastructure/ordering.h"
#include "supersonic/supersonic.h"
#include "supersonic/utils/strings/stringpiece.h"

#include "supersonic/utils/map_util.h"

#include <hash.h>

namespace supersonic {

template <int source, typename target>
struct QuicksaveCast;

template <>
struct QuicksaveCast<STRING, string>
{
    string operator()(const Column& column)
    {
        auto* stringPiece = column.typed_data<STRING>();
        if (stringPiece != nullptr)
        {
            return string(stringPiece->data(), stringPiece->size());
        }
        else
        {
            throw std::runtime_error("");
        }
    }
};

template <>
struct QuicksaveCast<INT32, int>
{
    int operator()(const Column& column)
    {
        auto* dataPtr = column.typed_data<INT32>();
        if (dataPtr != nullptr)
        {
            return *dataPtr;
        }
        else
        {
            throw std::runtime_error("");
        }
    }
};

template <int source, typename target>
struct QuicksaveOptionalCast;

template <>
struct QuicksaveOptionalCast<STRING, string>
{
    absl::optional<string> operator()(const Column& column)
    {
        if (column.is_null()[0])
        {
            return absl::nullopt;
        }
        else
        {
            auto* stringPiece = column.typed_data<STRING>();
            if (stringPiece != nullptr)
            {
                return string(stringPiece->data(), stringPiece->size());
            }
            else
            {
                throw std::runtime_error("");
            }
        }
    }
};
}