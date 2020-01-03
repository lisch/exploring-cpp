int foo(int x)
{
  return x;
}

int call(int (* const func)(int), int x)
{
    return func(x);
}
int main()
{
   return call(&foo, 42);
}
