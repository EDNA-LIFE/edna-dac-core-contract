/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
*/

#include "ednadactoken.hpp"
#include <math.h>

void ednadactoken::create(account_name issuer,
                       asset maximum_supply)
{
    require_auth(_self);

    auto sym = maximum_supply.symbol;
    eosio_assert(sym.is_valid(), "invalid symbol name");
    eosio_assert(maximum_supply.is_valid(), "invalid supply");
    eosio_assert(maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable(_self, sym.name());
    auto existing = statstable.find(sym.name());
    eosio_assert(existing == statstable.end(), "stake with symbol already exists");

    statstable.emplace(_self, [&](auto &s) {
        s.supply.symbol = maximum_supply.symbol;
        s.max_supply = maximum_supply;
        s.issuer = issuer;
    });
}

void ednadactoken::issue(account_name to, asset quantity, string memo)
{
    auto sym = quantity.symbol;
    eosio_assert(sym.is_valid(), "invalid symbol name");
    eosio_assert(memo.size() <= 256, "memo has more than 256 bytes");

    auto sym_name = sym.name();
    stats statstable(_self, sym_name);
    auto existing = statstable.find(sym_name);
    eosio_assert(existing != statstable.end(), "stake with symbol does not exist, create stake before issue");
    const auto &st = *existing;

    require_auth(st.issuer);
    eosio_assert(quantity.is_valid(), "invalid quantity");
    eosio_assert(quantity.amount > 0, "must issue positive quantity");

    eosio_assert(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
    eosio_assert(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify(st, 0, [&](auto &s) {
        s.supply += quantity;
    });

    add_balance(st.issuer, quantity, st.issuer);

    if (to != st.issuer)
    {
        SEND_INLINE_ACTION(*this, transfer, {st.issuer, N(active)}, {st.issuer, to, quantity, memo});
    }
}

void ednadactoken::transfer(account_name from,
                         account_name to,
                         asset quantity,
                         string memo)
{
    eosio_assert(from != to, "cannot transfer to self");
    require_auth(from);
    eosio_assert(is_account(to), "to account does not exist");
    auto sym = quantity.symbol.name();
    stats statstable(_self, sym);
    const auto &st = statstable.get(sym);

    require_recipient(from);
    require_recipient(to);

    eosio_assert(quantity.is_valid(), "invalid quantity");
    eosio_assert(quantity.amount > 0, "must transfer positive quantity");
    eosio_assert(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
    eosio_assert(memo.size() <= 256, "memo has more than 256 bytes");

    sub_balance(from, quantity);
    add_balance(to, quantity, from);
}

void ednadactoken::setmemfund(account_name _memfund)
{
    require_auth(_self);
    config_table c_t(_self, _self);
    auto c_itr = c_t.find(0);
    if (c_itr == c_t.end())
    {
        c_t.emplace(_self, [&](auto &c) {
            c.mem_fund = _memfund;
        });
    }
    else
    {
        c_t.modify(c_itr, _self, [&](auto &c) {
            c.mem_fund = _memfund;
        });
    }
}


/*******************************************************************************
 * Global Management *******************************************************
 *******************************************************************************/
//void ednadactoken::globalalarms(){
//    ednadactoken::general_proposal_check();
//    ednadactoken::memberhip_check();
//    ednadactoken::custodian_check();

//};



/*******************************************************************************
 * Membership Management *******************************************************
 *******************************************************************************/

void ednadactoken::addmember(account_name _account, string tele_user, asset quantity){
  require_auth(_account);
  config_table c_t (_self, _self);
  auto c_itr = c_t.find(0);
  account_name _mem_fund = c_itr->mem_fund;

  eosio_assert(tele_user != "", "telegram username may not be blank.");
  eosio_assert(c_itr->new_members_allowed != 0,"new membership is currently disabled.");

  member_table m_t(_self, _self);
  auto itr = m_t.find(_account);
  eosio_assert(itr == m_t.end(), "Account already is a DAC member.");

  auto sym = quantity.symbol.name();
  stats statstable(_self, sym);
  const auto &st = statstable.get(sym);

  eosio_assert(quantity.is_valid(), "invalid quantity");
  eosio_assert(quantity.amount > 0, "must transfer positive quantity");
  eosio_assert(quantity.symbol == st.supply.symbol, "symbol precision mismatch");

  sub_balance(_account, quantity);
  add_balance(_mem_fund, quantity, _account);

  m_t.emplace(_self, [&](auto &c) {
    c.member_id = m_t.available_primary_key();
    c.account = _account;
    c.member_status = MEM_MEMBER;
    c.custodial_status = CUSTO_NONE;
    c.telegram_user = tele_user;
    c.proposal_count = 0;
    c.vote_count = 0;
    c.completed_service_count = 0;
    c.completed_service_value = asset{static_cast<int64_t>(0), string_to_symbol(4, "EDNA")};
    c.research_value_earned = asset{static_cast<int64_t>(0), string_to_symbol(4, "EDNA")};
    c.total_value_earned = asset{static_cast<int64_t>(0), string_to_symbol(4, "EDNA")};
    c.member_balance = asset{static_cast<int64_t>(0), string_to_symbol(4, "EDNA")};;
    c.ipfs_member_bio = "";
    c.ipfs_member_photo = "";
    c.ipfs_member_video = "";
    c.ipfs_traits_data = "";
    c.ipfs_gen_data = "";
    c.joined_date = now();
    c.renewal_date = now() + c_itr->mem_ttl;
  });
}

void ednadactoken::deletemember(account_name _account){
  // archive members records...propsals, voting,

  // give back ram for voting table rease all rows for above




}

void ednadactoken::renew_membership(account_name _account){

  member_table m_t(_self, _self);
  auto itr = m_t.find(_account);
  eosio_assert(itr != m_t.end(), "member account does not exist.");

  config_table c_t (_self, _self);
  auto c_itr = c_t.find(0);
  asset renewal_fee = c_itr->mem_fee;
  account_name _mem_fund = c_itr->mem_fund;

  auto sym = renewal_fee.symbol.name();
  stats statstable(_self, sym);
  const auto &st = statstable.get(sym);

  eosio_assert(renewal_fee.is_valid(), "invalid quantity");
  eosio_assert(renewal_fee.amount > 0, "must transfer positive quantity");
  eosio_assert(renewal_fee.symbol == st.supply.symbol, "symbol precision mismatch");

  sub_balance(_account, renewal_fee);
  add_balance(_mem_fund, renewal_fee, _account);

    m_t.modify(itr, _self, [&](auto &c) {
      c.renewal_date += c.renewal_date + c_itr->mem_ttl;
    });
}

 void ednadactoken::updatemember(
   account_name _account,
   string _upd_type,
   uint8_t _param_i8,
   uint32_t _param32,
   uint64_t _parm64,
   string _param_s,
   asset _param_asset

  )
{
 member_table m_t(_self, _self);
 auto itr = m_t.find(_account);
 eosio_assert(itr != m_t.end(), "member account does not exist.");
 bool renewal_due = false;
 if(itr->renewal_date < now()){
   renewal_due = true;
 }
 eosio_assert(renewal_due == false, "membership expired, please renew.");

   m_t.modify(itr, _self, [&](auto &c) {
      if(_upd_type == "member_status"){
        c.member_status = _param_i8;
      }
      if(_upd_type == "custodial_status"){
        c.custodial_status = _param_i8;
      }
      if(_upd_type == "telegram_user"){
        c.telegram_user = _param_i8;
      }
      if(_upd_type == "proposal_count"){
        c.proposal_count += 1;
      }
      if(_upd_type == "vote_count"){
        c.vote_count += 1;
      }
      if(_upd_type == "completed_service"){
        c.completed_service_count += 1;
        c.completed_service_value += _param_asset;
        c.total_value_earned += _param_asset;
      }
      if(_upd_type == "research_value_earned"){
        c.completed_service_count += 1;
        c.research_value_earned += _param_asset;
        c.total_value_earned += _param_asset;
      }
      if(_upd_type == "member_balance" && _param_s == "add"){
        c.member_balance += _param_asset;
      }
      if(_upd_type == "member_balance" && _param_s == "rem"){
        c.member_balance -= _param_asset;
      }
      if(_upd_type == "ipfs_member_bio"){
        c.ipfs_member_bio = _param_s;
      }
      if(_upd_type == "ipfs_member_photo"){
        c.ipfs_member_photo = _param_s;
      }
      if(_upd_type == "ipfs_member_video"){
        c.ipfs_member_video = _param_s;
      }
      if(_upd_type == "ipfs_traits_data"){
        c.ipfs_traits_data = _param_s;
      }
      if(_upd_type == "ipfs_gen_data"){
        require_auth(_self);
        c.ipfs_traits_data = _param_s;
      }
    });
}


// Proposal management actions

void ednadactoken::new_general_proposal(account_name _from, string _title, string _text){

  uint64_t member_id;
  uint32_t mem_vote_ttl;

  member_table m_t(_self, _self);
  auto itr = m_t.find(_from);
  member_id = itr->member_id;

  config_table c_t (_self, _self);
  auto c_itr = c_t.find(0);
  mem_vote_ttl = c_itr->mem_vote_ttl + now();

  proposal_table p_t(_self, _self);
  p_t.emplace(_self, [&](auto &c) {

    c.prop_id = p_t.available_primary_key();
    c.sponsor_id = member_id;
    c.prop_type = GENERAL_PROPOSAL;
    c.prop_status = GEN_NEW;
    c.prop_title = _title;
    c.prop_ipfs_text = _text;
    c.prop_gen_total_votes = 0;
    c.prop_next_action_date = mem_vote_ttl;
  });
}

void ednadactoken::general_proposal_check(){



}

/*

const uint8_t   GEN_NEW = 1;            //time check
const uint8_t   GEN_UNSUPPORTED = 2;    //archive
const uint8_t   GEN_ESCALATED = 3;      //time check + flip type
const uint8_t   CUSTO_DEFEATED = 4;     //archive
const uint8_t   CUSTO_PASSED = 5;      //active archive
const uint8_t   CUSTO_STALLED_1 = 6;    //time check
const uint8_t   CUSTO_STALLED_2 = 7;    //time check
const uint8_t   CUSTO_STALLED_3 = 8;    //time check
const uint8_t   REF_CREATED = 9;        //time check
const uint8_t   REF_DEFEATED = 10;      //archive
const uint8_t   REF_PASSED = 11;        //active archive


   struct config {
     uint64_t      config_id;
     uint64_t      member_count;                                                // current active membership
     uint8_t       new_members_allowed = 1;                                     // non-zero = taking on new members
     uint8_t       new_members_minimum_stake = 1;                               // minimum stake to this contract for membership


     uint64_t      proposal_escalation = 40;                                    // % of members that must vote on a general proposal (+ or -) to auto-escalate to custodian action
     uint64_t      referendum_passage = 51;                                     // % of membership that must approve or defeat a referendum
     uint8_t       custodian_count = 12;
     uint8_t       custodial_majority = 9;                                      // 9 of 12 must approve or defeat a custodial vote

     uint32_t      mem_ttl = (60 * 60 * 24 * 365);                              // duration of membership - must renew after (-1 to disable renewals)
     uint32_t      mem_prop_ttl = (60 * 60 * 24 * 7);                           // time before a member proposal is removed or converted to a custodial vote
     uint32_t      custodian_vote_ttl = (60 * 60 * 24 * 2);                     // time betwwen stalled custodial votes
     uint64_t      nominations_ttl = (60 * 60 * 24 * 3);
     uint64_t      elections_ttl = (60 * 60 * 24 * 5);
     uint32_t      custodian_ttl = (60 * 60 * 24 * 30);                         // time between custodial elections

     uint64_t      next_cust_election_due;                                      // datetime of next election
     asset         dac_funds_main;
     asset         dac_funds_approved_spend;

*/








































void ednadactoken::sub_balance(account_name owner, asset value)
{
    accounts from_acnts(_self, owner);
    const auto &from = from_acnts.get(value.symbol.name(), "no balance object found");
    eosio_assert(from.balance.amount >= value.amount, "overdrawn balance");

    if (from.balance.amount == value.amount){
        from_acnts.erase(from);
    }
    else {
        from_acnts.modify(from, owner, [&](auto &a) {
            a.balance -= value;
        });
    }
}
/*
*   Add ballance can be sent here by anyone
*/
void ednadactoken::add_balance(account_name owner, asset value, account_name ram_payer)
{
    accounts to_acnts(_self, owner);
    auto to = to_acnts.find(value.symbol.name());
    if (to == to_acnts.end())
    {
        to_acnts.emplace(ram_payer, [&](auto &a) {
            a.balance = value;
        });
    }
    else
    {
        to_acnts.modify(to, 0, [&](auto &a) {
            a.balance += value;
        });
    }
}
