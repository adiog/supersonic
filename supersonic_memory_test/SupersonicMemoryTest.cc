#include <MetaBean.h>
#include <SupersonicMemory.h>
#include <gtest/gtest.h>
#include <qsql/qsqlSupersonicExpression.h>
#include <supersonic/cursor/infrastructure/view_cursor.h>

TEST(SupersonicMemoryTestSuite, BeanTestCase)
{
    auto memory = supersonic::SupersonicBeanTable<MetaBean>();

    {
        MetaBean bean;
        bean.meta_hash = "metahash1";
        bean.author = "author1";
        bean.name = "name1";
        bean.text = "text1";
        bean.meta_type = "type1";

        memory.insert(bean);
    }
    {
        MetaBean bean;
        bean.meta_hash = "metahash2";
        bean.author = "author2";
        bean.created_at = "created_at2";
        bean.name = "name2";
        bean.text = "text2";
        bean.meta_type = "type2";

        memory.insert(bean);
    }
    {
        MetaBean bean;
        bean.meta_hash = "metahash3";
        bean.author = "author3";
        bean.created_at = "created_at3";
        bean.name = "name3";
        bean.text = "text3";
        bean.meta_type = "type3";

        memory.insert(bean);
    }

    const auto *expression = QsqlSupersonicExpression::parseQsql("", "WHERE ((name = 'name1') or (text = 'text3'))");

    auto filtered = supersonic::Filter(
        expression,
        supersonic::ProjectAllAttributes(),
        memory.table.release()
    );

    supersonic::FailureOrOwned<supersonic::Cursor> cursor = filtered->CreateCursor();
    ASSERT_TRUE(cursor.is_success());

    supersonic::ResultView output = cursor.get()->Next(1);
    ASSERT_TRUE(output.has_data());
    auto& view = output.view();
    auto bean = memory.decode(view);
    std::cout << bean.to_string() << std::endl;

    supersonic::ResultView output2 = cursor.get()->Next(1);
    ASSERT_TRUE(output2.has_data());
    auto& view2 = output2.view();
    MetaBean bean2 = memory.decode(view2);
    std::cout << bean2.to_string() << std::endl;

    supersonic::ResultView output3 = cursor.get()->Next(1);
    ASSERT_FALSE(output3.has_data());
}