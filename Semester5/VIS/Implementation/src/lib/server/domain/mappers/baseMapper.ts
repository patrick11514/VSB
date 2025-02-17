export interface BaseMapper {
    toDomainModel(dto: unknown): unknown;
    toDTO(domain: unknown): unknown;
}
