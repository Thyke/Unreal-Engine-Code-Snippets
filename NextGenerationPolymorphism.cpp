// https://github.com/microsoft/proxy/blob/main/proxy.h

UFUNCTION(BlueprintCallable, Category = "ProxyTest")
	void TestProxy();

void ACharacter::TestProxy()
{
	// declare facade
	struct TestFacade : pro::facade_builder
		::add_convention<pro::operator_dispatch<"()", false>, int(int)>
		::build {};

	// create lambda
	auto lambda = [](int x) { return x * 2; };

	// create proxy
	pro::proxy<TestFacade> p = pro::make_proxy<TestFacade>(lambda);

	// uses proxy
	int result = (*p)(21);

	// Write log
	UE_LOG(LogTemp, Warning, TEXT("Proxy Test Result: %d"), result);
}
