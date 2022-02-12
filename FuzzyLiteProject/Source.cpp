#include <fl/Headers.h>

using namespace fl;


int main()
{
	Engine* engine = new Engine();
	engine->setName("Obstacle Avoidance");
	engine->setDescription("");

	InputVariable* obstacle = new InputVariable;
	obstacle->setName("obstacle");
	obstacle->setRange(0.000f, 1.000f);
	obstacle->addTerm(new Ramp("left", 1.000f, 0.000f));
	obstacle->addTerm(new Ramp("right", 0.000f, 1.000f));
	engine->addInputVariable(obstacle);

	InputVariable* speed = new InputVariable;
	speed->setName("speed");
	speed->setRange(0.000f, 1.000f);
	speed->addTerm(new Ramp("moving_left", 1.000f, 0.750f));
	//speed->setRange(0.250f, 1.000f);
	speed->addTerm(new Ramp("moving_right", 0.250f, 1.000f));
	//speed->setRange(0.100f, 0.900f);
	speed->addTerm(new Triangle("none", 0.000f, 1.000f, 0.000));
	engine->addInputVariable(speed);

	OutputVariable* mSteer = new OutputVariable;
	mSteer->setName("mSteer");
	mSteer->setRange(0.000f, 1.000f);
	mSteer->setAggregation(new Maximum);
	mSteer->setDefuzzifier(new Centroid(100));
	mSteer->setDefaultValue(fl::nan);
	mSteer->addTerm(new Ramp("left", 1.000f, 0.000f));
	mSteer->addTerm(new Ramp("right", 0.000f, 1.000f));
	engine->addOutputVariable(mSteer);

	RuleBlock* mamdani = new RuleBlock;
	mamdani->setName("mamdani");
	mamdani->setConjunction(new AlgebraicProduct);			// For AND...ing
	mamdani->setDisjunction(new Maximum);					// For OR...ing
	mamdani->setImplication(new AlgebraicProduct);			// If X then Y
	mamdani->setActivation(new General);					// Activates rules
	mamdani->addRule(Rule::parse("if obstacle is left and speed is moving_left then mSteer is right", engine));
	mamdani->addRule(Rule::parse("if obstacle is right and speed is moving_right then mSteer is left", engine));
	engine->addRuleBlock(mamdani);

	std::string status;

	if (!engine->isReady(&status))
	{
		std::cout << "Engine is not ready!\n";
		std::cout << "Status:\n" << status << '\n';
	}

	while (1)
	{
		std::string input = "";

		std::cin >> input;
		std::stringstream ssObs(input);
		float obsNumber = 0.0f;
		ssObs >> obsNumber;

		std::cin >> input;
		std::stringstream ssSpeed(input);
		float speedNumber = 0.0f;
		ssSpeed >> speedNumber;

		obstacle->setValue(obsNumber);
		speed->setValue(speedNumber);
		engine->process();

		std::cout << "obstacle.input = " << obsNumber << " => steer.output = " << mSteer->getValue() << '\n';
		std::cout << "speed.input = " << speedNumber << " => steer.output = " << mSteer->getValue() << '\n';
	}

	delete engine;
	return 0;
}