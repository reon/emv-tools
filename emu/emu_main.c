#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "emu_syntax.h"
#include "emu_ast.h"
#include "emu_glue.h"
#include "openemv/dump.h"

#include <stdio.h>
#include <stdbool.h>

void yyerror(YYLTYPE *yylloc, const char *name, struct emu_df **pdf, char *msg)
{
	fprintf(stderr, "%s:%d:%d: %s\n", name, yylloc->first_line, yylloc->first_column, msg);
}

int main(int argc, char **argv)
{
	struct emu_df *df;
	int ret;

	if (argc > 2)
		return 5;

	ret = yyparse(argc == 1 ? "-" : argv[1], &df);
	if (ret)
		return ret;

	const struct emu_property *prop = df_get_property(df, "name");
	if (!prop)
		return 6;

	const struct emu_value *value = property_get_value(prop, 1);
	if (!value)
		return 7;

	size_t buf_len;
	const unsigned char *buf = value_get(value, &buf_len);

	dump_buffer(buf, buf_len, stdout);

	df_free(df);

	return 0;
}