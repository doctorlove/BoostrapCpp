#include <algorithm>
#include <random>

#include "playing_cards.h"

namespace cards
{

	// Listing 5.10 Define operator << for a card, improved in Listing 5.12
	//std::ostream& operator<<(std::ostream& os, const Card& card)
	//{
	//	os << card.value << " of " << static_cast<int>(card.suit) << '\n';
	//	return os;
	//}

	//Listing 5.11 Turn an enum value into a string
	std::string to_string(Suit suit)
	{
		switch (suit)
		{
		case Suit::Heart:
			return "Hearts";
		case Suit::Diamond:
			return "Diamonds";
		case Suit::Club:
			return "Clubs";
		case Suit::Spade:
			return "Spades";
		default:
			return "?";
		}
	}

	// Listing 5.12 Use the enum name rather than value, improved on in listing 5.14
	//std::ostream& operator<<(std::ostream& os, const Card& card)
	//{
	//	os << card.value << " of " << to_string(card.suit);
	//	return os;
	//}
	//Listing 5.13 Convert card value to a string
	std::string card_value_to_string(int value)
	{
		using namespace std::literals;

			switch (value)
			{
			case 1:
				return "Ace"s;
			case 11:
				return "Jack"s;
			case 12:
				return "Queen"s;
			case 13:
				return "King"s;
			default:
				return std::to_string(value);
			}
	}

	//Listing 5.14 Show ace, jack, queen, king or number
	std::ostream& operator<<(std::ostream& os, const Card& card)
	{
		os << card_value_to_string(card.value)
			<< " of " << to_string(card.suit);
		return os;
	}


	// Listing 5.15 Build a deck of cards, given as
	// std::array<Card, 52> create_deck() in the book
	std::array<Card, 52> create_deck_first_way()
	{
		std::array<Card, 52> deck;
		auto card = deck.begin();
		for (auto suit :
			{ Suit::Heart, Suit::Diamond, Suit::Club, Suit::Spade })
		{
			for (int value = 1; value <= 13; value++)
			{
				card->suit = suit;
				card->value = value;
				++card;
			}
		}
		return deck;
	}

	// Listing 5.16 increment our enum
	Suit& operator++(Suit& suit)
	{
		// Dangerous! See https://stackoverflow.com/questions/3475152/why-cant-i-increment-a-variable-of-an-enumerated-type
		using IntType = typename std::underlying_type<Suit>::type;
		if (suit == Suit::Spade)
			suit = Suit::Heart;
		else
			suit = static_cast<Suit>(static_cast<IntType>(suit) + 1);
		return suit;
	}

	//Listing 5.17 Generating the deck of cards
	std::array<Card, 52> create_deck()
	{
		std::array<Card, 52> deck;
		int value = 1;
		Suit suit = Suit::Heart;
		std::ranges::generate(deck, [&value, &suit]() {
			if (value > 13)
			{
				value = 1;
				++suit;
			}
		return Card{ value++, suit };
		});
		return deck;
	}

	// Listing 5.20 Shuffle the cards
	void shuffle_deck(std::array<Card, 52>& deck)
	{
		std::random_device rd;
		std::mt19937 gen{ rd() };
		std::ranges::shuffle(deck, gen);
	}

	// Listing 5.21 Is the guess correct?
	bool is_guess_correct(char guess, const Card & current, const Card & next)
	{
		return (guess == 'h' && next > current) 
			    || (guess == 'l' && next < current);
	}

	//Listing 5.22 Higher lower game
	void higher_lower()
	{
		auto deck = create_deck();
		shuffle_deck(deck);

		size_t index = 0;
		while (index + 1 < deck.size())
		{
			std::cout << deck[index]
				<< ": Next card higher (h) or lower (l)?\n>";
			char c;
			std::cin >> c;
			bool ok = is_guess_correct(c, deck[index], deck[index + 1]);
			if (!ok)
			{
				std::cout << "Next card was " << deck[index + 1] << '\n';
				break;
			}
			++index;
		}
		std::cout << "You got " << index << " guess correct\n";
	}

	//Listing 5.26 Creating an extended deck
	std::array<std::variant<Card, Joker>, 54> create_extended_deck()
	{
		std::array<std::variant<Card, Joker>, 54> deck{ Joker{} , Joker{} };
		std::array<Card, 52> cards = create_deck();
		std::ranges::copy(cards, std::begin(deck) + 2);
		return deck;
	}

	//Listing 5.27 Shuffle an extended deck
	void shuffle_deck(std::array<std::variant<Card, Joker>, 54>& deck)
	{
		std::random_device rd;
		std::mt19937 gen{ rd() };
		std::ranges::shuffle(deck, gen);
	}

	//Listing 5.28 Is the guess correct for an extended deck
	bool is_guess_correct(char c,
		const std::variant<Card, Joker>& current,
		const std::variant<Card, Joker>& next)
	{
		if (std::holds_alternative<Joker>(current) || std::holds_alternative<Joker>(next))
			return true;
		Card current_card = std::get<Card>(current);
		Card next_card = std::get<Card>(next);
		return is_guess_correct(c, current_card, next_card);
	}

	//Listing 5.29 Stream out cards and jokers
	std::ostream& operator<<(std::ostream& os, const std::variant<Card, Joker>& card)
	{
		if (std::holds_alternative<Joker>(card))
			os << "JOKER";
		else
			os << std::get<Card>(card);
		return os;
	}

	//Listing 5.30 Higher lower game with Jokers too
	void higher_lower_with_jokers()
	{
		auto deck = create_extended_deck();
		shuffle_deck(deck);

		size_t index = 0;
		while (index + 1 < deck.size())
		{
			std::cout << deck[index]
				<< ": Next card higher (h) or lower (l)?\n>";
			char c;
			std::cin >> c;
			bool ok = is_guess_correct(c, deck[index], deck[index + 1]);
			if (!ok)
			{
				std::cout << "Next card was " << deck[index + 1] << '\n';
				break;
			}
			++index;
		}
		std::cout << "You got " << index << " guess correct\n";
	}
}