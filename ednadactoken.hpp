// header file for EDNA DAC

/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <string>

using namespace eosio;
using std::string;
using eosio::const_mem_fun;

class ednadactoken : public contract
{
  public:
    ednadactoken(account_name self) : contract(self) {}

    inline asset get_supply(symbol_name sym) const;

    inline asset get_balance(account_name owner, symbol_name sym) const;

    // @abi action
    void create(account_name issuer, asset maximum_supply);

    // @abi action
    void issue(account_name to, asset quantity, string memo);

    // @abi action
    void setmemfund(account_name _memfund);

    // @abi action
    void addmember(account_name _account, string tele_user, asset quantity);

    // @abi action
    void deletemember(account_name _account);

    // @abi action
    void renew_membership(account_name _account);

    // @abi action
  //  void join(account_name name);

    // @abi action
    void updatemember(account_name _account, string _upd_type, uint8_t _param_i8, uint32_t _param32, uint64_t _parm64, string _param_s, asset _param_asset);

    // @abi action
    void new_general_proposal(account_name _from, string _title, string _text);

    // @abi action
    void general_proposal_check();

    // @abi action
    void transfer(account_name from, account_name to, asset quantity, string memo);


  private:

    // types of votes & proposals
    const uint8_t   GENERAL_PROPOSAL = 1;
    const uint8_t   CUSTODIAL_MATTER = 2;
    const uint8_t   MEMBER_REFERENDUM = 3;
    const uint8_t   SERVICE_PROPOSAL = 4;
    const uint8_t   RESEARCH_PROPOSAL = 5;
    const uint8_t   IMPEACHMENT_PROCEEDING = 6;

    // membership statuses
    const uint8_t   MEM_MEMBER = 1;
    const uint8_t   MEM_PAID_KIT = 2;
    const uint8_t   MEM_KIT_SHIPPED = 3;
    const uint8_t   MEM_KIT_IN_LAB = 4;
    const uint8_t   MEM_DNA_ON_CHAIN = 5;
    const uint8_t   MEM_LIFETIME = 6;
    const uint8_t   MEM_SUSPENDED = 7;
    const uint8_t   MEM_BANNED = 8;
  	const uint8_t   MEM_ABANDONED = 9;

    // Vote statuses
    const uint8_t   GEN_NEW = 1;
    const uint8_t   GEN_UNSUPPORTED = 2;
    const uint8_t   GEN_ESCALATED = 3;
    const uint8_t   CUSTO_DEFEATED = 4;
    const uint8_t   CUSTO_PASSED = 5;
    const uint8_t   CUSTO_STALLED_1 = 6;
    const uint8_t   CUSTO_STALLED_2 = 7;
    const uint8_t   CUSTO_STALLED_3 = 8;
    const uint8_t   REF_CREATED = 9;
    const uint8_t   REF_DEFEATED = 10;
    const uint8_t   REF_PASSED = 11;

    // service propsal statuses
    const uint8_t   SERVE_NEW = 1;
    const uint8_t   SERVE_DENIED = 2;
    const uint8_t   SERVE_APPROVED = 3;
    const uint8_t   SERVE_APPROVED_WITH_MOD = 4;
    const uint8_t   SERVE_MOD_ACCEPTED = 5;
    const uint8_t   SERVE_MOD_REJECTED = 6;
    const uint8_t   SERVE_IN_PROGRESS = 7;
    const uint8_t   SERVE_COMPLETED = 8;
    const uint8_t   SERVE_PAY_SETTLED = 9;
    const uint8_t   SERVE_UNDELIVERED = 10;

    // research propsal statuses
    const uint8_t   	RESEARCH_NEW = 1;
    const uint8_t   	RESEARCH_UNDER_REVIEW = 2;
    const uint8_t   	RESEARCH_DENIED = 3;
    const uint8_t   	RESEARCH_APPROVED = 4;
    const uint8_t	    RESEARCH_ACCEPTING_OPT_IN = 5;
    const uint8_t   	RESEARCH_IN_PROGRESS = 6;
    const uint8_t   	RESEARCH_COMPLETE = 7;
    const uint8_t   	RESEARCH_PAID = 8;
    const uint8_t     RESEARCH_PMT_DISTRIBUTED = 9;

    // election statuses
    const uint8_t   ELECT_OFF = 1;
    const uint8_t   ELECT_NOMINATING = 2;
    const uint8_t   ELECT_VOTING = 3;
    const uint8_t   ELECT_TABULATING = 4;
    const uint8_t   ELECT_HISTORY = 5;

    // custodian statuses
    const uint8_t   CUSTO_NONE = 1;
    const uint8_t   CUSTO_NOMINATED = 2;
    const uint8_t   CUSTO_DECLINED = 3;
    const uint8_t   CUSTO_RUNNING = 4;
    const uint8_t   CUSTO_DEFETED = 5;
    const uint8_t   CUSTO_SITTING = 6;                                          // auto nominated
    const uint8_t   CUSTO_REMOVED = 7;                                          // can not be nominated
    const uint8_t   CUSTO_RETIRED = 8;                                          // can not be nominated


// TABLE STRUCTURES

  // @abi table members i64
  struct member{
      uint64_t          	member_id;
      account_name      	account;
      uint8_t           	member_status;                                        // see possible statuses above
      uint8_t             custodial_status;                                     // see possible statuses above
      string            	telegram_user;
      uint32_t            proposal_count;
      uint32_t            vote_count;
      uint32_t            completed_service_count;
      uint32_t            research_opt_in_count;
      asset               completed_service_value;
      asset               research_value_earned;
      asset               total_value_earned;
      asset               member_balance;
      string              ipfs_member_bio;
      string              ipfs_member_photo;
      string              ipfs_member_video;
      string              ipfs_traits_data;
      string              ipfs_gen_data;
      uint32_t          	joined_date;
      uint32_t            renewal_date;
      uint64_t            spare1;
      uint64_t            spare2;
      string              spare3;
      asset               spare4;


  uint64_t      primary_key() const { return member_id; }

  EOSLIB_SERIALIZE (member, (member_id)(account)(member_status)(custodial_status)(telegram_user)(proposal_count)
  (vote_count)(completed_service_count)(completed_service_value)(research_opt_in_count)(research_value_earned)(total_value_earned)(member_balance)
  (ipfs_traits_data)(ipfs_gen_data)(joined_date)(spare1)(spare2)(spare3)(spare4));
};
typedef eosio::multi_index<N(members), member> member_table;


  // @abi table propsals i64
  struct proposal {
    uint64_t          prop_id;
    uint64_t          sponsor_id;                                               //points to member_id
    uint8_t           prop_type;                                                //points to const proposal types
    uint8_t           prop_status;                                              //points to const propsal statuses
    string            prop_title;
    string            prop_ipfs_text;                                           //ipfs storage hash string
    uint32_t          prop_gen_total_votes;
    uint32_t          prop_cus_total_votes;
    uint32_t          prop_ref_total_votes;
    uint64_t          prop_next_action_date;

    uint64_t      primary_key() const { return prop_id; }
    uint64_t      by_status() const {return prop_status;}

    EOSLIB_SERIALIZE (proposal, (prop_id)(sponsor_id)(prop_type)(prop_status)(prop_title)(prop_ipfs_text)(prop_next_action_date));
  };

typedef eosio::multi_index<N(proposals), proposal,
    indexed_by<N(status),const_mem_fun<proposal, uint64_t, &proposal::by_status>>>proposal_table;

  // @abi action
  void proposal_by_type(uint8_t _type);

  // @abi table services i64
  struct service {
    uint64_t          	service_id;
    uint64_t          	member_id;
    uint8_t            	service_status;
    string            	ipfs_service_descr;
    asset              	service_cost;
    uint64_t           	start_date;
    uint64_t          	projected_end_date;
    uint64_t           	actual_end_date;
    uint8_t           	average_rating;
    uint32_t          	times_rated;

    uint64_t      primary_key() const { return service_id; }

    EOSLIB_SERIALIZE (service, (service_id)(member_id)(service_status)(ipfs_service_descr)(service_cost)(start_date)(projected_end_date)
    (actual_end_date)(average_rating)(times_rated));
  };

typedef eosio::multi_index<N(services), service> service_table;


  struct research {
    uint64_t 			research_id;
    uint64_t 			edna_sponsor_id;
    string  			project_title;
    uint8_t			  research_status;
    string  			company_entity;
    string  			contact;
    string  			email;
    string  			telephone;
    string  			website;
    uint64_t  		number_genomes_sought;
    uint64_t			number_of_opt_ins;
    uint8_t  			genome_type;
    string  			ipfs_hash_traits_sought;
    string			  ipfs_hash_funding_source_disclosure;
    string 			  ipfs_hash_research_purpose;
    string  			ipfs_hash_publications_list;
    string  			ipfs_hash_notes_comments;
    asset  			  proposed_payment;
    asset				  surety_bond_posted;
    uint8_t  			profit_sharing_offered;
    uint8_t  			agree_terms_of_service;
    uint8_t  			agree_edna_intended_use_policy;
    uint8_t  			agree_edna_constitution;
    uint8_t  			agree_eos_constitution;

    uint64_t      primary_key() const { return research_id; }
    EOSLIB_SERIALIZE (research, (research_id)(edna_sponsor_id)(project_title)(research_status)(company_entity)(contact)(email)(telephone)(website)
    (number_genomes_sought)(number_of_opt_ins)(genome_type)(ipfs_hash_traits_sought)(ipfs_hash_funding_source_disclosure)(ipfs_hash_research_purpose)
    (ipfs_hash_publications_list)(ipfs_hash_notes_comments)(proposed_payment)(surety_bond_posted)(profit_sharing_offered)
    (agree_terms_of_service)(agree_edna_intended_use_policy)(agree_edna_constitution)(agree_eos_constitution));
  };

  typedef eosio::multi_index<N(researches), research> research_table;

  // @abi table services i64
  struct research_committee {
      uint64_t			committee_id;
      uint64_t			research_id;
      uint64_t			chairman_id;
      uint64_t			member1_id;
      uint64_t			member2_id;
      uint64_t			member3_id;
      uint64_t			member4_id;
      uint64_t			member5_id;
      uint64_t			member6_id;
      uint64_t			member7_id;
      uint64_t			member8_id;
      uint64_t			member9_id;
      uint64_t			member10_id;
      uint64_t			member11_id;

    uint64_t      primary_key() const { return committee_id; }
    EOSLIB_SERIALIZE (research_committee, (committee_id)(research_id)(chairman_id)(member1_id)(member2_id)(member3_id)(member4_id)(member5_id)
    (member6_id)(member7_id)(member8_id)(member9_id)(member10_id)(member11_id));

};
typedef eosio::multi_index<N(research_committees), research_committee> research_committee_table;

  // @abi table services i64
  struct research_member_x_ref {
      uint64_t			research_id;
      uint64_t			member_id;

    uint64_t      primary_key() const { return research_id; }
    EOSLIB_SERIALIZE (research_member_x_ref, (research_id)(member_id));
};

typedef eosio::multi_index<N(research_member_x_refs), research_member_x_ref> research_member_x_ref_table;

// @abi table services i64
struct election{
  uint64_t            election_id;
  uint8_t             election_status;
  uint32_t          	next_action_date;

uint64_t      primary_key() const { return election_id; }
EOSLIB_SERIALIZE (election, (election_id)(election_status)(next_action_date));
};

typedef eosio::multi_index<N(elections), election> election_table;


// @abi table services i64
struct election_member_xref{
  uint64_t            elect_member_xref_id;
  uint64_t            election_id;
  uint64_t            memeber_id;
  uint32_t            votes;
  uint8_t             elected;

uint64_t  primary_key() const { return elect_member_xref_id; }
EOSLIB_SERIALIZE (election_member_xref, (elect_member_xref_id)(election_id)(memeber_id)(votes)(elected));
};

typedef eosio::multi_index<N(election_member_xrefs), election_member_xref> election_member_xref_table;

  // @abi table services i64
struct vote {
  uint64_t        vote_id;
  uint64_t        prop_elect_id;
  uint8_t         prop_type;
  uint64_t        member_id;
  uint8_t         how_vote;

  uint64_t  primary_key() const { return vote_id; }

  EOSLIB_SERIALIZE (vote, (vote_id)(prop_elect_id)(prop_type)(member_id)(how_vote));
  };

typedef eosio::multi_index<N(votes), vote> vote_table;





    struct config {
      uint64_t      config_id;
      uint64_t      member_count;                                               // current active membership
      uint8_t       new_members_allowed = 1;                                    // non-zero = taking on new members
      uint32_t      mem_ttl = (60 * 60 * 24 * 365);                             // duration of membership - must renew after (-1 to disable renewals)
      uint64_t      proposal_escalation = 40;                                   // % of members that must vote on a general proposal (+ or -) to auto-escalate to custodian action
      uint32_t      mem_vote_ttl = (60 * 60 * 24 * 7);                          // time before a member proposal is removed or converted to a custodial vote
      uint32_t      custodian_vote_ttl = (60 * 60 * 24 * 2);                    // time betwwen stalled custodial votes
      uint64_t      referendum_passage = 51;                                    // % of membership that must approve or defeat a referendum
      uint64_t      nominations_ttl = (60 * 60 * 24 * 3);
      uint64_t      elections_ttl = (60 * 60 * 24 * 5);
      uint8_t       custodian_count = 12;
      uint8_t       custodial_majority = 9;                                     // 9 of 12 must approve or defeat a custodial vote
      uint32_t      custodian_ttl = (60 * 60 * 24 * 30);                        // time between custodial elections
      uint64_t      next_cust_election_due;                                     // datetime of next election
      asset         dac_funds_main;
      asset         dac_funds_approved_spend;
      asset         mem_fee = asset{static_cast<int64_t>(1), string_to_symbol(4, "EDNA")};
      account_name  mem_fund;                                              // minimum fee to this contract for membership
      uint64_t      spare1;
      uint64_t      spare2;
      uint64_t      spare3;

      uint64_t      primary_key() const { return config_id; }

      EOSLIB_SERIALIZE (config, (config_id)(member_count)(new_members_allowed)(mem_fee)(mem_ttl)(nominations_ttl)(elections_ttl)
      (proposal_escalation)(mem_vote_ttl)(custodial_majority)(referendum_passage)(custodian_ttl)(custodian_vote_ttl)(next_cust_election_due)
      (dac_funds_main)(dac_funds_approved_spend)(mem_fund)(spare1)(spare2)(spare3));
  };

  typedef eosio::multi_index<N(configs), config> config_table;





// @abi table accounts i64
struct account
{
    asset balance;
    uint64_t primary_key() const { return balance.symbol.name(); }

    EOSLIB_SERIALIZE (account, (balance));
};



// @abi table stat i64
struct currencystat
{
    asset supply;
    asset max_supply;
    account_name issuer;

    uint64_t primary_key() const { return supply.symbol.name(); }

    EOSLIB_SERIALIZE (currencystat, (supply)(max_supply)(issuer));
};

typedef eosio::multi_index<N(accounts), account> accounts;
typedef eosio::multi_index<N(stat), currencystat> stats;

void sub_balance(account_name owner, asset value);
void add_balance(account_name owner, asset value, account_name ram_payer);

void sub_stake(account_name owner, asset value);
void add_stake(account_name owner, asset value);


struct transfer_args
{
    account_name from;
    account_name to;
    asset quantity;
    string memo;
};
};


asset ednadactoken::get_supply(symbol_name sym) const
{
stats statstable(_self, sym);
const auto &st = statstable.get(sym);
return st.supply;
}

asset ednadactoken::get_balance(account_name owner, symbol_name sym) const
{
accounts accountstable(_self, owner);
const auto &ac = accountstable.get(sym);
return ac.balance;
}









EOSIO_ABI( ednadactoken,(addmember)(deletemember)(renew_membership)(updatemember)(new_general_proposal)
(general_proposal_check)(transfer))
