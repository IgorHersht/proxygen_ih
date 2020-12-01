
template<class T, size_t N>
constexpr size_t ArraySize(T (&)[N]) { return N; }

int main()
{
	int ar[] = {1,2};
	int ar2[ArraySize(ar)];
	return 0;
}
