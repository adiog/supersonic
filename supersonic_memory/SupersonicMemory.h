// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include "supersonic/cursor/core/sort.h"
#include "supersonic/cursor/infrastructure/ordering.h"
#include "supersonic/supersonic.h"
#include "supersonic/utils/strings/stringpiece.h"

#include "supersonic/utils/map_util.h"

#include <MetaBean.h>
#include <hash.h>

namespace supersonic {

  template<int source, typename target>
  struct QuicksaveOptionalCast;

  template<>
  struct QuicksaveOptionalCast<STRING, string>{
    absl::optional<string> operator()(const Column& column) {
        if (column.is_null()[0]) {
            return absl::nullopt;
        } else {
          auto *stringPiece = column.typed_data<STRING>();
          if (stringPiece != nullptr){
            return string(stringPiece->data(), stringPiece->size());
        }else {
            throw std::runtime_error("");
            }
        }
    }
  };

  template<typename MetaBean>
  struct SupersonicBeanTable {
    TupleSchema schema;
    std::unique_ptr<Table> table;
    std::unique_ptr<TableRowWriter> table_writer;

    SupersonicBeanTable()
    {
      schema.add_attribute(Attribute("meta_hash", STRING, NULLABLE));
      schema.add_attribute(Attribute("author", STRING, NULLABLE));
      schema.add_attribute(Attribute("created_at", STRING, NULLABLE));
      schema.add_attribute(Attribute("icon", STRING, NULLABLE));
      schema.add_attribute(Attribute("meta_type", STRING, NULLABLE));
      schema.add_attribute(Attribute("modified_at", STRING, NULLABLE));
      schema.add_attribute(Attribute("name", STRING, NULLABLE));
      schema.add_attribute(Attribute("source_title", STRING, NULLABLE));
      schema.add_attribute(Attribute("source_url", STRING, NULLABLE));
      schema.add_attribute(Attribute("text", STRING, NULLABLE));
      schema.add_attribute(Attribute("user_hash", STRING, NULLABLE));

      table.reset(new Table(schema, HeapBufferAllocator::Get()));

      table_writer.reset(new TableRowWriter(table.get()));
    }

    void insert(MetaBean& bean)
    {
      if (!bean.meta_hash) bean.meta_hash = qs::util::Hash::get();

      auto& add_row = table_writer->AddRow();

      if (bean.meta_hash) { add_row.String((ValueRef<STRING>(*bean.meta_hash)).value()); } else { add_row.Null(); }
      if (bean.author) { add_row.String((ValueRef<STRING>(*bean.author)).value()); } else { add_row.Null(); }
      if (bean.created_at) { add_row.String((ValueRef<STRING>(*bean.created_at)).value()); } else { add_row.Null(); }
      if (bean.icon) { add_row.String((ValueRef<STRING>(*bean.icon)).value()); } else { add_row.Null(); }
      if (bean.meta_type) { add_row.String((ValueRef<STRING>(*bean.meta_type)).value()); } else { add_row.Null(); }
      if (bean.modified_at) { add_row.String((ValueRef<STRING>(*bean.modified_at)).value()); } else { add_row.Null(); }
      if (bean.name) { add_row.String((ValueRef<STRING>(*bean.name)).value()); } else { add_row.Null(); }
      if (bean.source_title) { add_row.String((ValueRef<STRING>(*bean.source_title)).value()); } else { add_row.Null(); }
      if (bean.source_url) { add_row.String((ValueRef<STRING>(*bean.source_url)).value()); } else { add_row.Null(); }
      if (bean.text) { add_row.String((ValueRef<STRING>(*bean.text)).value()); } else { add_row.Null(); }
      if (bean.user_hash) { add_row.String((ValueRef<STRING>(*bean.user_hash)).value()); } else { add_row.Null(); }

      add_row.CheckSuccess();
    }

    MetaBean decode(const View& view)
    {
      MetaBean bean;

      bean.meta_hash = QuicksaveOptionalCast<STRING, string>()(view.column(0));
      bean.author = QuicksaveOptionalCast<STRING, string>()(view.column(1));
      bean.created_at = QuicksaveOptionalCast<STRING, string>()(view.column(2));
      bean.icon = QuicksaveOptionalCast<STRING, string>()(view.column(3));
      bean.meta_type = QuicksaveOptionalCast<STRING, string>()(view.column(4));
      bean.modified_at = QuicksaveOptionalCast<STRING, string>()(view.column(5));
      bean.name = QuicksaveOptionalCast<STRING, string>()(view.column(6));
      bean.source_title = QuicksaveOptionalCast<STRING, string>()(view.column(7));
      bean.source_url = QuicksaveOptionalCast<STRING, string>()(view.column(8));
      bean.text = QuicksaveOptionalCast<STRING, string>()(view.column(9));
      bean.user_hash = QuicksaveOptionalCast<STRING, string>()(view.column(10));

      return std::move(bean);
    }
  };



}