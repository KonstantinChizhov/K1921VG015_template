

namespace std
{
    void __throw_out_of_range_fmt(const char *fmt, ...)
    {
         (void)fmt;
        while(1);
    }

    void __throw_length_error(const char *err)
    {
        (void)err;
        while(1);
    }

    void __throw_bad_function_call()
	{
		while(1);
	}

}