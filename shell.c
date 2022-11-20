#include "shell.h"
#include "string.h"
#include "fat.h"
#include "lib/print.h"

const char *prompt = "shell> ";

struct command
{
	const char *name;
	void (*function)(const char *param);
};

static struct command commands[42] = {
	{
		.name = "ls",
		.function = print_dir
	},
	{
		.name = "cat",
		.function = print_file
	}
};

static char buffer[20];
static unsigned buffer_idx;

static void reset_buffer(void)
{
	for (unsigned i = 0; i < sizeof buffer; ++i)
		buffer[i] = 0;
	buffer_idx = 0;
}

void shell_reset(void)
{
	reset_buffer();
	printk("%s", prompt);
}

void shell_feed_char(char c)
{
	if (buffer_idx >= sizeof buffer)
		shell_reset();
	buffer[buffer_idx++] = c;
	if ('\n' != c)
		return;
	for (struct command* cmdp = commands; cmdp->name; cmdp++)
		if (0 == strncmp(cmdp->name, buffer, strlen(cmdp->name)))
		{
			cmdp->function(buffer + strlen(cmdp->name) + 1);
			break;
		}
	shell_reset();
}

