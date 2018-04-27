//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_SNDENUMS_HPP
#define NOI_SOFTWARE_SNDENUMS_HPP
namespace NSnd {
/*----------------------------------------------------------------------*/
    enum ETones {
        Cn1, Csn1, Dn1, Dsn1, En1, Fn1, Fsn1, Gn1, Gsn1, An1, Asn1, Bn1,
        C0, Cs0, D0, Ds0, E0, F0, Fs0, G0, Gs0, A0, As0, B0,
        C1, Cs1, D1, Ds1, E1, F1, Fs1, G1, Gs1, A1, As1, B1,
        C2, Cs2, D2, Ds2, E2, F2, Fs2, G2, Gs2, A2, As2, B2,
        C3, Cs3, D3, Ds3, E3, F3, Fs3, G3, Gs3, A3, As3, B3,
        C4, Cs4, D4, Ds4, E4, F4, Fs4, G4, Gs4, A4, As4, B4,
        C5, Cs5, D5, Ds5, E5, F5, Fs5, G5, Gs5, A5, As5, B5,
        C6, Cs6, D6, Ds6, E6, F6, Fs6, G6, Gs6, A6, As6, B6,
        C7, Cs7, D7, Ds7, E7, F7, Fs7, G7, Gs7, A7, As7, B7,
        C8, Cs8, D8, Ds8, E8, F8, Fs8, G8, Gs8, A8, As8, B8,
    };

/*----------------------------------------------------------------------*/
    enum EMidiMsgType {
        NOTE_OFF, NOTE_ON, BEND_ON, BEND_OFF, ALL_OFF
    };
}
#endif //NOI_SOFTWARE_SNDENUMS_HPP
